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
	
	void OnDeleteUnsuedAssetButtonClicked(); // ������� �ʴ� Asset ����
	void OnDeleteEmptyFoldersButtonClicked(); // ������� �ʴ� Folder ����
	void OnAdvanceDeletionButtonClicked(); // ���� ������

	void FixUpRedirectors(); // Fix Up Redirectors 

#pragma endregion

#pragma region CustomEditorTab

	void RegisterAdvanceDeletionTab();

	TSharedRef<SDockTab> OnSpawnAdvanceDeltionTab(const FSpawnTabArgs& SpawnTabArgs); // Tab ����

	TArray< TSharedPtr <FAssetData> > GetAllAssetDataUnderSelectedFolder(); // ������ ������ ���Ե� ��� ���� �ҷ�����

#pragma endregion

public:
#pragma region ProccessDataForAdvanceDeletionTab

	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);

#pragma endregion

};
