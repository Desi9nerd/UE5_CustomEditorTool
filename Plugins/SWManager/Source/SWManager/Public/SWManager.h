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
	TSharedPtr<SDockTab> ConstructedDockTab;

	TArray< TSharedPtr <FAssetData> > GetAllAssetDataUnderSelectedFolder(); // ������ ������ ���Ե� ��� ���� �ҷ�����

	void OnAdvanceDeletionTabClosed(TSharedRef<SDockTab> TabToClose);

#pragma endregion

// ������ Actor���� Lock/Unlock �ϱ�
#pragma region LevelEditorMenuExtension

	void InitLevelEditorExtention(); // LevelEditor MenuExtention�� CustomLevelEditorMenuExtender ��� 

	TSharedRef<FExtender> CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors); // AddLevelEditorMenuEntry() �߰�

	void AddLevelEditorMenuEntry(class FMenuBuilder& MenuBuilder); // OnLockActorSelectionButtonClicked()�� OnUnlockActorSelectionButtonClicked() �߰�

	void OnLockActorSelectionButtonClicked(); // Lock �ɱ�. void LockActorSelection()ȣ��
	void OnUnlockActorSelectionButtonClicked(); // Unlock �ϱ�. void UnlockActorSelection()ȣ��

#pragma endregion

// World Outliner�� Column�� ����
#pragma region SceneOutlinerExtension

	void InitSceneOutlinerColumnExtension();

	TSharedRef<class ISceneOutlinerColumn> OnCreateSelectionLockColumn(class ISceneOutliner& SceneOutliner);

	void UnRegisterSceneOutlinerColumnExtension(); // Module�� �������� Unregister

#pragma endregion

// Actor�� ���õǾ����� �Ͼ�� Delegate Event
#pragma region SelectionLock

	void InitCustomSelectionEvent(); // Actor�� ���õǾ����� Delegate Event�� �Լ�ȣ��
	
	void OnActorSelected(UObject* SelectedObject); // Delegate Event Callback�Լ�

	void LockActorSelection(TObjectPtr<AActor> ActorToProcess); 
	void UnlockActorSelection(TObjectPtr<AActor> ActorToProcess);

	void RefreshSceneOutliner(); // ContentBrowserMenuExtension�� World Outliner�� ǥ�õǴ� Lock/Unlock ��� ����ȭ

#pragma endregion

// ����Ű �������� Lock/Unlock ����
#pragma region CustomEditorUICommands

	TSharedPtr<class FUICommandList> CustomUICommands; // ����Ű

	void InitCustomUICommands();

	void OnSelectionLockHotKeyPressed(); // Lock ����Ű ����
	void OnUnlockActorSelectionHotKeyPressed(); // Unlock ����Ű ����

#pragma endregion

	TWeakObjectPtr<class UEditorActorSubsystem> WeakEditorActorSubsystem;
	bool GetEditorActorSubsystem(); 


public:
#pragma region ProccessDataForAdvanceDeletionTab

	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);
	bool DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete); // �Ű������� ���� ����TArray�迭�� �ѹ��� ����� true����. �ƴϸ� false����.
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData); // ������� �ʴ� ���� ����Ʈ ������Ʈ
	void ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData); // ���� �̸��� ����ϴ� ���� ����Ʈ ������Ʈ
	void SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync); // Ŭ���� ������ ��θ� ��ũ

#pragma endregion

	bool CheckIsActorSelectionLocked(TObjectPtr<AActor> ActorToProcess); // ������ Actor�� "Locked" tag�� ������ true, ������ false
	void ProcessLockingForOutliner(TObjectPtr<AActor> ActorToProcess, bool bShouldLock);
};
