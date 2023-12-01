#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSWManagerModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

#pragma region ContentBrowserMenuExtention

	void InitCBMenuExtention();

	TArray<FString> FolderPathsSelected;

	TSharedRef<FExtender> CustomCBMenuExtender(const TArray<FString>& SelectedPaths);

	void AddCBMenuEntry(class FMenuBuilder& MenuBuilder);
	
	void OnDeleteUnsuedAssetButtonClicked(); // 사용하지 않는 Asset 제거
	void OnDeleteEmptyFoldersButtonClicked(); // 사용하지 않는 Folder 제거
	void OnAdvanceDeletionButtonClicked(); // 제거 마법사

	void FixUpRedirectors(); // Fix Up Redirectors 

#pragma endregion

#pragma region CustomEditorTab

	void RegisterAdvanceDeletionTab();

	TSharedRef<SDockTab> OnSpawnAdvanceDeltionTab(const FSpawnTabArgs& SpawnTabArgs);

#pragma endregion

};
