﻿#include "SWManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FSWManagerModule"

void FSWManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// SWManager.uplugin에서 설정한 PreDefault 이후 실행됨
	InitCBMenuExtention();
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
		FSlateIcon(), // Custom Icon
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteUnsuedAssetButtonClicked) // 실행될 함수
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("사용하지 않는 폴더 제거하기")), // Menu Entry의 이름
		FText::FromString(TEXT("빈 폴더를 안전하게 지우는 기능")), // Tooltip 설명
		FSlateIcon(), // Custom Icon
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteEmptyFoldersButtonClicked) // 실행될 함수
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

void FSWManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSWManagerModule, SWManager)