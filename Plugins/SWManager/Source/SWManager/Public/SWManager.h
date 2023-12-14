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

	TSharedRef<SDockTab> OnSpawnAdvanceDeltionTab(const FSpawnTabArgs& SpawnTabArgs); // Tab 스폰
	TSharedPtr<SDockTab> ConstructedDockTab;

	TArray< TSharedPtr <FAssetData> > GetAllAssetDataUnderSelectedFolder(); // 선택한 폴더에 포함된 모든 에셋 불러오기

	void OnAdvanceDeletionTabClosed(TSharedRef<SDockTab> TabToClose);

#pragma endregion

// 선택한 Actor들을 Lock/Unlock 하기
#pragma region LevelEditorMenuExtension

	void InitLevelEditorExtention(); // LevelEditor MenuExtention에 CustomLevelEditorMenuExtender 등록 

	TSharedRef<FExtender> CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors); // AddLevelEditorMenuEntry() 추가

	void AddLevelEditorMenuEntry(class FMenuBuilder& MenuBuilder); // OnLockActorSelectionButtonClicked()와 OnUnlockActorSelectionButtonClicked() 추가

	void OnLockActorSelectionButtonClicked(); // Lock 걸기. void LockActorSelection()호출
	void OnUnlockActorSelectionButtonClicked(); // Unlock 하기. void UnlockActorSelection()호출

#pragma endregion

// World Outliner의 Column에 노출
#pragma region SceneOutlinerExtension

	void InitSceneOutlinerColumnExtension();

	TSharedRef<class ISceneOutlinerColumn> OnCreateSelectionLockColumn(class ISceneOutliner& SceneOutliner);

	void UnRegisterSceneOutlinerColumnExtension(); // Module이 끝났을때 Unregister

#pragma endregion

// Actor가 선택되었을때 일어나는 Delegate Event
#pragma region SelectionLock

	void InitCustomSelectionEvent(); // Actor가 선택되었을때 Delegate Event로 함수호출
	
	void OnActorSelected(UObject* SelectedObject); // Delegate Event Callback함수

	void LockActorSelection(TObjectPtr<AActor> ActorToProcess); 
	void UnlockActorSelection(TObjectPtr<AActor> ActorToProcess);

	void RefreshSceneOutliner(); // ContentBrowserMenuExtension과 World Outliner에 표시되는 Lock/Unlock 기능 동기화

#pragma endregion

// 단축키 눌렸을때 Lock/Unlock 실행
#pragma region CustomEditorUICommands

	TSharedPtr<class FUICommandList> CustomUICommands; // 단축키

	void InitCustomUICommands();

	void OnSelectionLockHotKeyPressed(); // Lock 단축키 눌림
	void OnUnlockActorSelectionHotKeyPressed(); // Unlock 단축키 눌림

#pragma endregion

	TWeakObjectPtr<class UEditorActorSubsystem> WeakEditorActorSubsystem;
	bool GetEditorActorSubsystem(); 


public:
#pragma region ProccessDataForAdvanceDeletionTab

	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);
	bool DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete); // 매개변수로 받은 에셋TArray배열을 한번에 지우고 true리턴. 아니면 false리턴.
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData); // 사용하지 않는 에셋 리스트 업데이트
	void ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData); // 같은 이름을 사용하는 에셋 리스트 업데이트
	void SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync); // 클릭한 에셋의 경로를 싱크

#pragma endregion

	bool CheckIsActorSelectionLocked(TObjectPtr<AActor> ActorToProcess); // 선택한 Actor에 "Locked" tag가 있으면 true, 없으면 false
	void ProcessLockingForOutliner(TObjectPtr<AActor> ActorToProcess, bool bShouldLock);
};
