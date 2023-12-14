#pragma once
#include "ISceneOutlinerColumn.h"

/** Custom World Outliner 만들기
 *  World Outliner의 Column에 Lock/Unlock 기능 넣기
 *  ISceneOutlinerColumn을 상속받고 함수들을 override하여 사용함. ISceneOutlinerColumn를 상속받아 사용하려면 .Build.cs에 "SceneOutliner" 추가하여야 함.
 */
class FOutlinerSelectionColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionColumn(ISceneOutliner& SceneOutliner) {}

	virtual FName GetColumnID() override { return FName("SelectionLock"); }
	static FName GetID() { return FName("SelectionLock"); }

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef< SWidget > ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;

private:
	void OnRowWidgetCheckStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> CorrespondingActor);
};