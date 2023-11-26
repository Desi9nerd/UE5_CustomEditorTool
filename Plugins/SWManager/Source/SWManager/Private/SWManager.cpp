#include "SWManager.h"
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

	// ContentBrowserModuleMenuExtenders에 항목을 추가하고 FContentBrowserMenuExtender_SelectedPaths에 this(=FSWManagerModule)를 생성하고 CustomCBMenuExtender함수를 Bind 한다.
	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSWManagerModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FSWManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths) // 우클릭 시 ContentBrowserMenu에 노출시켜질 위치
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		// MenuExtender의 "Delete"위치 뒤에 UICommandList를 추가하고 AddCBMenuEntry함수를 Bind 한다.
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FSWManagerModule::AddCBMenuEntry));

		FolderPathsSelected = SelectedPaths; // 유저가 현재 선택한 폴더에 접근할 수 있도록 SelectedPaths를 FolderPathsSelected변수에 담는다
	}

	return MenuExtender;
}

void FSWManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder) // ContentBrowserMenu에 노출되는 버튼에 함수 바인딩
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("사용하지 않은 에셋 제거하기")),
		FText::FromString(TEXT("폴더에서 사용하지 않는 에셋들을 안전하게 지우는 기능")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteUnsuedAssetButtonClicked)
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

	if (AssetsPathNames.Num() == 0) // 폴더를 선택하지 않은 경우
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택한 폴더에서 아무런 에셋을 찾을 수 없습니다."));
		return;
	}

	EAppReturnType::Type ConfirmResult =DebugHeader::ShowMsgDialog(EAppMsgType::YesNo, TEXT("총  ") + FString::FromInt(AssetsPathNames.Num()) + TEXT(" 개의 에셋을 찾았습니다.\n삭제를 진행하시겠습니까?"));

	if (ConfirmResult == EAppReturnType::No) return;

	FixUpRedirectors();

	TArray<FAssetData> UnusedAssetsDataArray; // 사용되지 않는 에셋들을 담는 TArray변수

	for (const FString& AssetPathName : AssetsPathNames)
	{
		// Root 폴더는 건드리면 안 된다. 프로젝트 파일 Content 밑의 Developer, Collection 폴더일 경우 continue
		if (AssetPathName.Contains(TEXT("Developers")) || 	AssetPathName.Contains(TEXT("Collections")))
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
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택한 폴더에서 사용하지 않는 에셋을 찾을 수 없습니다. 삭제할 수 없습니다.\n 에셋이 없거나 사용중입니다."));
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