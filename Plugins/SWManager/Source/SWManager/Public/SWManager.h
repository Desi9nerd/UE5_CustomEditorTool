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

	TArray< TSharedPtr <FAssetData> > GetAllAssetDataUnderSelectedFolder(); // 선택한 폴더에 포함된 모든 에셋 불러오기

#pragma endregion

#pragma region LevelEditorMenuExtension

	void InitLevelEditorExtention(); // LevelEditor MenuExtention에 CustomLevelEditorMenuExtender 등록 

	TSharedRef<FExtender> CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors); // AddLevelEditorMenuEntry() 추가

	void AddLevelEditorMenuEntry(class FMenuBuilder& MenuBuilder); // OnLockActorSelectionButtonClicked()와 OnUnlockActorSelectionButtonClicked() 추가

	void OnLockActorSelectionButtonClicked(); // Lock 걸기
	void OnUnlockActorSelectionButtonClicked(); // Unlock 하기

#pragma endregion

public:
#pragma region ProccessDataForAdvanceDeletionTab

	bool DeleteSingleAssetForAssetList(const FAssetData& AssetDataToDelete);
	bool DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete); // 매개변수로 받은 에셋TArray배열을 한번에 지우고 true리턴. 아니면 false리턴.
	void ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData); // 사용하지 않는 에셋 리스트 업데이트
	void ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData); // 같은 이름을 사용하는 에셋 리스트 업데이트
	void SyncCBToClickedAssetForAssetList(const FString& AssetPathToSync); // 클릭한 에셋의 경로를 싱크

#pragma endregion

};
