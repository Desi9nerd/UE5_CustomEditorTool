#pragma once
#include "ISceneOutlinerColumn.h"

/** Custom World Outliner 만들기
 *  ISceneOutlinerColumn을 상속받고 함수들을 override하여 사용함
 */
class FOutlinerSelectionLockColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutliner) {}

	virtual FName GetColumnID() override { return FName("SelectionLock"); }

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef< SWidget > ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;
};