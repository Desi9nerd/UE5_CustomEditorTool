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

#pragma region LevelEditorMenuExtension

	void InitLevelEditorExtention(); // LevelEditor MenuExtention�� CustomLevelEditorMenuExtender ��� 

	TSharedRef<FExtender> CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors); // AddLevelEditorMenuEntry() �߰�

	void AddLevelEditorMenuEntry(class FMenuBuilder& MenuBuilder); // OnLockActorSelectionButtonClicked()�� OnUnlockActorSelectionButtonClicked() �߰�

	void OnLockActorSelectionButtonClicked(); // Lock �ɱ�
	void OnUnlockActorSelectionButtonClicked(); // Unlock �ϱ�

#pragma endregion

public:
#pragma region ProccessDataForAdvanceDeletionTab

	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);
	bool DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete); // �Ű������� ���� ����TArray�迭�� �ѹ��� ����� true����. �ƴϸ� false����.
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData); // ������� �ʴ� ���� ����Ʈ ������Ʈ
	void ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData); // ���� �̸��� ����ϴ� ���� ����Ʈ ������Ʈ
	void SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync); // Ŭ���� ������ ��θ� ��ũ

#pragma endregion

};
