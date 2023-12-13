#include "SWManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "CustomStyle/SWManagerStyle.h"
#include "LevelEditor.h" //
#include "Engine/Selection.h" // GEditor->GetSelectedActors();에 사용
#include "Subsystems/EditorActorSubsystem.h"
#include "CustomUICommands/SWManagerUICommands.h"

#define LOCTEXT_NAMESPACE "FSWManagerModule"

void FSWManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// SWManager.uplugin에서 설정한 PreDefault 이후 실행됨

	FSWManagerStyle::InitializeIcons(); // Custom Icon Init

	InitCBMenuExtention(); // Content Browser Menu Extention Init

	RegisterAdvanceDeletionTab(); // Tab 스폰 시키기

	FSWManagerUICommands::Register(); // SWManagerUICommands클래스 Register. 단축키 등록하기

	InitLevelEditorExtention();

	InitCustomSelectionEvent();
}

#pragma region ContentBrowserMenuExtention

void FSWManagerModule::InitCBMenuExtention()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser")); // ContentBrowser 모듈 로드하여 변수에 담음

	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
		ContentBrowserModule.GetAllPathViewContextMenuExtenders(); // ContentBrowserModule의 모든ContentMenuExtenders를 TArray변수에 담음

	/*FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FSWManagerModule::CustomCBMenuExtender);
	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate); //아래의 한줄과 같은 의미 */

	// ContentBrowserModuleMenuExtenders에 항목을 추가하고 FContentBrowserMenuExtender_SelectedPaths에 this(=FSWManagerModule)를 생성하고 CustomCBMenuExtender함수를 Bind 한다. 다른말로 표현하면, 현재 존재하는 모든 delegates들에 내가 만든 CustomCBMenuExtender 델리게이트를 추가한다.
	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSWManagerModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FSWManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths) // 우클릭 시 ContentBrowserMenu에 노출시켜질 위치를 정의해주기
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		// MenuExtender의 "Delete"위치 뒤에 UICommandList를 추가하고 AddCBMenuEntry함수를 Bind 한다.
		MenuExtender->AddMenuExtension(FName("Delete"), // Extension hook, 삽입할 위치
			EExtensionHook::After,						// Delete 뒤에 들어가게 After 설정
			TSharedPtr<FUICommandList>(),				// Custom hot keys
			FMenuExtensionDelegate::CreateRaw(this, &FSWManagerModule::AddCBMenuEntry)); // Second binding, Menu Entry에 노출될 디테일을 정의해줄 AddCBMenuEntry를 바인딩

		FolderPathsSelected = SelectedPaths; // 유저가 현재 선택한 폴더에 접근할 수 있도록 SelectedPaths를 FolderPathsSelected변수에 담는다
	}

	return MenuExtender;
}

void FSWManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder) // ContentBrowserMenu에 노출되는 버튼에 함수 바인딩
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("사용하지 않는 에셋 제거하기")), // Menu Entry의 이름
		FText::FromString(TEXT("폴더에서 사용하지 않는 에셋들을 안전하게 지우는 기능")), // Tooltip 설명
		FSlateIcon(FSWManagerStyle::GetStyleSetName(), "ContentBrowser.DeleteUnusedAssets"), // Custom Icon. "ContentBrowser.DeleteUnusedAssets"이름의 Style 적용
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteUnsuedAssetButtonClicked) // 실행될 함수
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("사용하지 않는 빈 폴더 제거하기")), // Menu Entry의 이름
		FText::FromString(TEXT("빈 폴더를 안전하게 지우는 기능")), // Tooltip 설명
		FSlateIcon(FSWManagerStyle::GetStyleSetName(), "ContentBrowser.DeleteEmptyFolders"), // Custom Icon
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteEmptyFoldersButtonClicked) // 실행될 함수
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("제거 마법사")), // Menu Entry의 이름
		FText::FromString(TEXT("탭에 제거할 에셋 목록 띄우기")), // Tooltip 설명
		FSlateIcon(FSWManagerStyle::GetStyleSetName(), "ContentBrowser.AdvanceDeletion"), // Custom Icon
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnAdvanceDeletionButtonClicked) // 실행될 함수
	);
}

void FSWManagerModule::OnDeleteUnsuedAssetButtonClicked()  // 에셋 삭제
{
	if (FolderPathsSelected.Num() > 1) // 2개 이상의 폴더가 선택된 경우
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("한 개의 폴더에서만 진행할 수 있습니다"));
		return;
	}

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]); // 선택된 폴더(FolderPathsSelected[0]) 내에 있는 모든 에셋들을 AssetsPathNames변수에 담는다

	if (AssetsPathNames.Num() == 0) // 폴더 내에서 에셋을 찾지 못한 경우
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택한 폴더에서 아무런 에셋을 찾을 수 없습니다."), false);
		return;
	}

	EAppReturnType::Type ConfirmResult =DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("총  ") + FString::FromInt(AssetsPathNames.Num()) + TEXT(" 개의 에셋을 찾았습니다.\n사용하지 않는지 체크 후 삭제를 진행하시겠습니까?"), false);

	if (ConfirmResult == EAppReturnType::No) return; // 유저가 No를 누르면 삭제를 진행하지 않고 바로 종료

	FixUpRedirectors(); // 경로가 문제되지 않도록 체크 후 업데이트

	TArray<FAssetData> UnusedAssetsDataArray; // 사용되지 않는 에셋들을 담는 TArray변수

	for (const FString& AssetPathName : AssetsPathNames)
	{
		// Root 폴더는 건드리면 안 된다. 프로젝트 파일 Content 밑의 Developer, Collection 폴더일 경우 continue
		if (AssetPathName.Contains(TEXT("Developers")) || 	AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) || 	AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		// 에셋이 없다면 continue
		if (false == UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		// AssetReferencer변수에 referencers들의 package path를 담는다. 에셋 데이터가 아닌 path를 담는것이다.
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);

		if (AssetReferencers.Num() == 0) // 해당 에셋이 사용되고 있지 않다면
		{
			// 에셋 데이터를 UnusedAssetsDataArray(=사용되지 않는 에셋들을 담는 TArray변수)에 추가
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetsDataArray.Add(UnusedAssetData);
		}
	}

	if (UnusedAssetsDataArray.Num() > 0) // 사용되지 않는 에셋이 0이상이라면
	{
		ObjectTools::DeleteAssets(UnusedAssetsDataArray); // UnusedAssetsDataArray를 삭제
	}
	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택한 폴더에서 사용하지 않는 에셋을 찾을 수 없습니다. 삭제할 수 없습니다.\n 에셋이 없거나 사용중입니다."), false);
	}
}

void FSWManagerModule::OnDeleteEmptyFoldersButtonClicked() // 사용하지 않는 Folder 제거
{
	FixUpRedirectors(); // 경로가 문제되지 않도록 체크 후 업데이트
	
	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0], true, true); // 선택된 폴더(FolderPathsSelected[0]) 내에 있는 모든 에셋+폴더들을 FolderPathsArray변수에 담는다
	uint32 Counter = 0;

	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFoldersPathsArray; // 빈 폴더들을 담을 TArray변수

	for (const FString& FolderPath : FolderPathsArray)
	{
		// Root 폴더는 건드리면 안 된다. 프로젝트 파일 Content 밑의 Developer, Collection, __ExternalActors__, __ExternalObjects__폴더일 경우 continue
		if (FolderPath.Contains(TEXT("Developers")) || FolderPath.Contains(TEXT("Collections")) ||
			FolderPath.Contains(TEXT("__ExternalActors__")) || FolderPath.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		// 폴더가 없다면 continue
		if (false == UEditorAssetLibrary::DoesDirectoryExist(FolderPath)) continue;

		// FolderPath에 에셋이 없다면
		if (false == UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
		{
			EmptyFolderPathsNames.Append(FolderPath);
			EmptyFolderPathsNames.Append(TEXT("\n"));

			EmptyFoldersPathsArray.Add(FolderPath); // EmptyFoldersPathsArray배열에 빈 폴더의 FolderPath를 추가
		}
	}

	if (EmptyFoldersPathsArray.Num() == 0) // 빈 폴더가 없다면
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택한 폴더들 중 빈 폴더를 찾지 못 했습니다."), false);
		return;
	}

	EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::OkCancel, TEXT("빈 폴더  ") + EmptyFolderPathsNames + TEXT(" 를 찾았습니다. \n 모두 삭제 하시겠습니까?"), false);

	if (ConfirmResult == EAppReturnType::Cancel) return; // Cancel를 누르면 삭제하지 않고 종료

	for (const FString& EmptyFolderPath : EmptyFoldersPathsArray)
	{
		UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath) ? ++Counter : DebugHeader::Print(TEXT("폴더 " + EmptyFolderPath + " 삭제를 실패했습니다."), FColor::Red); // 디버깅 메시지 띄우기
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("폴더 ") + FString::FromInt(Counter) + TEXT("개를 성공적으로 삭제했습니다."));
	}
}

void FSWManagerModule::OnAdvanceDeletionButtonClicked() // 제거 마법사
{
	FixUpRedirectors();

	FGlobalTabmanager::Get()->TryInvokeTab(FName("AdvanceDeletion")); // Menu Entry에 "AdvanceDeletion"란 Tab ID를 가진 탭 부르기. "AdvanceDeletion"가 오타나면 실행되지 않는다.
}

void FSWManagerModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	FARFilter Filter;
	Filter.bRecursivePaths = true; // subfolder에 접근이 가능하도록 true 설정
	Filter.PackagePaths.Emplace("/Game"); // 어떤 폴더에 접근할 지 경로 설정
	Filter.ClassNames.Emplace("ObjectRedirector"); // 필터 하기를 희망하는 클래스의 이름

	TArray<FAssetData> OutRedirectors; // 결과를 저장할 TArry 변수
	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectors); // Filter한 결과를 OutRedirectors에 담는다

	for (const FAssetData& RedirectorData : OutRedirectors)
	{
		if (UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset())) // Redirect 할게 있다면
		{
			RedirectorsToFixArray.Add(RedirectorToFix); // RedirectorsToFixArray에 RedirectorToFix 담음
		}
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}

#pragma endregion

#pragma region CustomEditorTab

void FSWManagerModule::RegisterAdvanceDeletionTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AdvanceDeletion"), // Tab ID: "AdvanceDeletion"
		FOnSpawnTab::CreateRaw(this, &FSWManagerModule::OnSpawnAdvanceDeltionTab))
		.SetDisplayName(FText::FromString(TEXT("제거 마법사")))
		.SetIcon(FSlateIcon(FSWManagerStyle::GetStyleSetName(), "ContentBrowser.AdvanceDeletion"));
}

TSharedRef<SDockTab> FSWManagerModule::OnSpawnAdvanceDeltionTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return
	SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SAdvanceDeletionTab)
		.AssetsDataToStore(GetAllAssetDataUnderSelectedFolder()) // 선택한 폴더에 포함된 모든 에셋 불러오기
		.CurrentSelectedFolder(FolderPathsSelected[0]) // 현재 폴더
	];
}

TArray<TSharedPtr<FAssetData>> FSWManagerModule::GetAllAssetDataUnderSelectedFolder()
{
	TArray< TSharedPtr <FAssetData> > AvaiableAssetsData; // 유효한 에셋들

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]); // 선택된 폴더(FolderPathsSelected[0]) 내에 있는 모든 에셋들을 AssetsPathNames변수에 담는다

	for (const FString& AssetPathName : AssetsPathNames)
	{
		// Root folder들은 건드리지 않는다
		if (AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		// 에셋이 없다면 continue
		if (false == UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		// 에셋을 찾아 Data변수에 담는다
		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);

		// MakeShared<FAssetData>(Data)를 AvaiableAssetsData에 추가
		AvaiableAssetsData.Add(MakeShared<FAssetData>(Data));
	}

	return AvaiableAssetsData; // 유효한 에셋들 리턴
}

#pragma endregion

#pragma region ProccessDataForAdvanceDeletionTab

bool FSWManagerModule::DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete) // 에셋 1개 제거
{
	TArray<FAssetData> AssetDataForDeletion;
	AssetDataForDeletion.Add(AssetDataToDelete); // 제거할 에셋을 추가

	// AssetDataForDeletion에 추가된 에셋을 ObjectTools::DeleteAssets()으로 제거
	if (ObjectTools::DeleteAssets(AssetDataForDeletion) > 0)
	{
		return true; // 제거한 에셋이 하나라도 있으면 true 리턴
	}

	return false;
}

bool FSWManagerModule::DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete) // 에셋들 한번에 제거
{
	// AssetsToDelete에 추가된 에셋들을 ObjectTools::DeleteAssets()으로 제거
	if (ObjectTools::DeleteAssets(AssetsToDelete) > 0)
	{
		return true;
	}

	return false;
}

void FSWManagerModule::ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData) // 사용하지 않는 에셋 리스트 업데이트
{
	OutUnusedAssetsData.Empty(); // OutUnusedAssetsData변수 초기화

	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray<FString> AssetReferencers = UEditorAssetLibrary::FindPackageReferencersForAsset(DataSharedPtr->ObjectPath.ToString()); // DataSharedPtr 에셋이 참조되었는지 검사

		if (AssetReferencers.Num() == 0) // 에셋이 참조된 수가 0이라면(=사용하지 않는 에셋이라면)
		{
			OutUnusedAssetsData.Add(DataSharedPtr); // 해당 에셋 데이터를 OutUnusedAssetsData에 추가
		}
	}
}

void FSWManagerModule::ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData)
{
	OutSameNameAssetsData.Empty();

	// Multimap을 사용하여 같은 이름을 가진 에셋들을 찾는다
	TMultiMap<FString, TSharedPtr<FAssetData>> AssetsInfoMultiMap;

	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		AssetsInfoMultiMap.Emplace(DataSharedPtr->AssetName.ToString(), DataSharedPtr); // 모든 에셋을 AssetsInfoMultiMap에 담는다. 이 때 key는 AssetName이다.
	}

	for (const TSharedPtr<FAssetData>& DataSharedPtr : AssetsDataToFilter)
	{
		TArray< TSharedPtr <FAssetData> > OutAssetsData;
		AssetsInfoMultiMap.MultiFind(DataSharedPtr->AssetName.ToString(), OutAssetsData);

		if (OutAssetsData.Num() <= 1) continue; // AssetsInfoMultiMap에서 AssetName이 1개란 의미는 이름이 같은 에셋이 없다는 의미

		// 같은 이름이지만 다른 에셋 데이터를 가지고 있다면 OutSameNameAssetsData에 담는다
		for (const TSharedPtr<FAssetData>& SameNameData : OutAssetsData)
		{
			if (SameNameData.IsValid())
			{
				OutSameNameAssetsData.AddUnique(SameNameData); // 같은 이름을 가진 에셋을 OutSameNameAssetsData에 담는다
			}
		}
	}
}

void FSWManagerModule::SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync)
{
	TArray<FString> AssetsPathToSync;
	AssetsPathToSync.Add(AssetPathToSync);

	UEditorAssetLibrary::SyncBrowserToObjects(AssetsPathToSync); // 선택한 에셋을 콘텐츠 브라우저에서 찾아 활성화 한다
}

#pragma endregion

#pragma region LevelEditorMenuExtension

void FSWManagerModule::InitLevelEditorExtention() // Actor의 MenuExtender에 CustomLevelEditorMenuExtender 등록
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor")); // LevelEditorModule

	TArray<FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors>& LevelEditorMenuExtenders = LevelEditorModule.GetAllLevelViewportContextMenuExtenders(); // Actor를 우클릭할 때 나타나는 MenuExtender 모두 담기

	LevelEditorMenuExtenders.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this, &FSWManagerModule::CustomLevelEditorMenuExtender)); // MenuExtender에 CustomLevelEditorMenuExtender를 추가
}

TSharedRef<FExtender> FSWManagerModule::CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors)
{
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender()); // Extender 생성

	if (SelectedActors.Num() > 0)
	{
		// ActorOptions 위치 전에 AddLevelEditorMenuEntry를 추가
		MenuExtender->AddMenuExtension(
			FName("ActorOptions"),
			EExtensionHook::Before,
			UICommandList,
			FMenuExtensionDelegate::CreateRaw(this, &FSWManagerModule::AddLevelEditorMenuEntry)
		);
	}

	return MenuExtender;
}

void FSWManagerModule::AddLevelEditorMenuEntry(FMenuBuilder& MenuBuilder) // OnLockActorSelectionButtonClicked()와 OnUnlockActorSelectionButtonClicked() 추가
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("선택한 Actor 모두 Lock 걸기")),
		FText::FromString(TEXT("Actor가 선택되는 것을 방지한다.")),
		FSlateIcon(FSWManagerStyle::GetStyleSetName(), "LevelEditor.LockSelection"), // "LevelEditor.LockSelection"이름의 Style 적용
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnLockActorSelectionButtonClicked)
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("선택한 Actor 모두 Unlock 하기")),
		FText::FromString(TEXT("Lock된 Actor들 잠금 해제")),
		FSlateIcon(FSWManagerStyle::GetStyleSetName(), "LevelEditor.UnlockSelection"),// "LevelEditor.UnlockSelection"이름의 Style 적용
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnUnlockActorSelectionButtonClicked)
	);
}

void FSWManagerModule::OnLockActorSelectionButtonClicked() // Lock 걸기
{
	if (false == GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = WeakEditorActorSubsystem->GetSelectedLevelActors();

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("선택한 Actor가 없습니다."));
		return;
	}

	FString CurrentLockedActorNames = TEXT("선택한 Actor들 Lock 걸기: ");

	for (AActor* SelectedActor : SelectedActors)
	{
		if (false == IsValid(SelectedActor)) continue;

		LockActorSelection(SelectedActor);

		WeakEditorActorSubsystem->SetActorSelectionState(SelectedActor, false); // SelectedActor 선택 해제

		CurrentLockedActorNames.Append(TEXT("\n"));
		CurrentLockedActorNames.Append(SelectedActor->GetActorLabel()); // SelectedActor 이름 추가
	}

	DebugHeader::ShowNotifyInfo(CurrentLockedActorNames); // 에디터 우측하단에 Lock걸 Actor들의 이름들을 디버깅 메시지로 띄우기
}

void FSWManagerModule::OnUnlockActorSelectionButtonClicked() // Unlock 하기
{
	if (false == GetEditorActorSubsystem()) return;

	TArray<AActor*> AllActorsInLevel = WeakEditorActorSubsystem->GetAllLevelActors(); // Level에 배치된 모든 Actor를 담음
	TArray<AActor*> AllLockedActors; // Lock된 Actors 모두 담을 TArray배열변수

	//** Level에 배치된 모든 Actor들 중 Lock된 Actor들을 AllLockedActors에 담음
	for (AActor* ActorInLevel : AllActorsInLevel)
	{
		if (false == IsValid(ActorInLevel)) continue;

		if (CheckIsActorSelectionLocked(ActorInLevel)) // ActorInLevel이 Lock된 Actor라면
		{
			AllLockedActors.Add(ActorInLevel); // Lock된 ActorInLevel 모두 담음
		}
	}

	if (AllLockedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("현재 선택된 Actor들 중 Lock된게 없습니다."));
	}

	FString UnlockedActorNames = TEXT("선택한 Actor들 Unlock 하기: ");

	for (AActor* LockedActor : AllLockedActors)
	{
		UnlockActorSelection(LockedActor);

		UnlockedActorNames.Append(TEXT("\n"));
		UnlockedActorNames.Append(LockedActor->GetActorLabel()); // LockedActor 이름 추가
	}

	DebugHeader::ShowNotifyInfo(UnlockedActorNames); // 에디터 우측하단에 UnLock될 Actor들의 이름들을 디버깅 메시지로 띄우기
}

#pragma endregion

#pragma region SelectionLock

void FSWManagerModule::InitCustomSelectionEvent() // Actor가 선택되었을때 Delegate Event로 함수호출
{
	TObjectPtr<USelection> UserSelection = GEditor->GetSelectedActors(); // #include "Engine/Selection.h" 필요

	//** Custom Binding: Actor가 선택되는 이벤트가 발생하면 OnActiorSelected()함수 호출
	UserSelection->SelectObjectEvent.AddRaw(this, &FSWManagerModule::OnActorSelected);
}

void FSWManagerModule::OnActorSelected(UObject* SelectedObject)
{
	if (false == GetEditorActorSubsystem()) return;

	if (TObjectPtr<AActor> SelectedActor = Cast<AActor>(SelectedObject)) // 선택한 Object가 Actor라면
	{
		if (CheckIsActorSelectionLocked(SelectedActor)) // SelectedActor에 "Locked" tag가 있다면
		{			
			WeakEditorActorSubsystem->SetActorSelectionState(SelectedActor, false); // SelectedActor 선택 해제
		}
	}
}

void FSWManagerModule::LockActorSelection(TObjectPtr<AActor> ActorToProcess)
{
	if (false == IsValid(ActorToProcess)) return;

	if (false == ActorToProcess->ActorHasTag(FName("Locked"))) // "Locked" tag가 없다면
	{
		ActorToProcess->Tags.Add(FName("Locked")); // "Locked" tag 추가
	}
}

void FSWManagerModule::UnlockActorSelection(TObjectPtr<AActor> ActorToProcess)
{
	if (false == IsValid(ActorToProcess)) return;

	if (ActorToProcess->ActorHasTag(FName("Locked"))) // "Locked" tag가 있다면
	{
		ActorToProcess->Tags.Remove(FName("Locked")); // "Locked" tag 제거
	}
}

// 매개변수로 들어오는 ActorToProcess에 "Locked" tag가 있으면 true, 없으면 false
bool FSWManagerModule::CheckIsActorSelectionLocked(TObjectPtr<AActor> ActorToProcess) 
{
	if (false == IsValid(ActorToProcess)) return false;

	return ActorToProcess->ActorHasTag(FName("Locked"));
}

#pragma endregion

bool FSWManagerModule::GetEditorActorSubsystem() 
{
	// WeakEditorActorSubsystem변수에 UEditorActorSubsystem을 담고 true리턴
	if (false == WeakEditorActorSubsystem.IsValid())
	{
		WeakEditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>(); // #include "Subsystems/EditorActorSubsystem.h" 필요
	}

	return WeakEditorActorSubsystem.IsValid();
}


void FSWManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(FName("AdvanceDeletion")); // Tab ID: "AdvanceDeletion"

	FSWManagerStyle::ShutDown(); // Custom Icon Shutdown
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSWManagerModule, SWManager)