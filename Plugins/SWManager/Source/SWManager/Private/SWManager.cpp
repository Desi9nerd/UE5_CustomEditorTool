#include "SWManager.h"
#include "ContentBrowserModule.h"

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

TSharedRef<FExtender> FSWManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		// MenuExtender의 "Delete"위치 뒤에 UICommandList를 추가하고 AddCBMenuEntry함수를 Bind 한다.
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FSWManagerModule::AddCBMenuEntry));
	}

	return MenuExtender;
}

void FSWManagerModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("사용하지 않은 에셋 제거하기")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FSWManagerModule::OnDeleteUnsuedAssetButtonClicked)
	);
}

void FSWManagerModule::OnDeleteUnsuedAssetButtonClicked()
{
}

#pragma endregion

void FSWManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSWManagerModule, SWManager)