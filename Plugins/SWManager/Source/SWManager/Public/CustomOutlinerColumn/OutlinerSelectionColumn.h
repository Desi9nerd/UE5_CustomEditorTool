#pragma once
#include "ISceneOutlinerColumn.h"

/** Custom World Outliner �����
 *  ISceneOutlinerColumn�� ��ӹް� �Լ����� override�Ͽ� �����
 */
class FOutlinerSelectionLockColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutliner) {}

	virtual FName GetColumnID() override { return FName("SelectionLock"); }

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef< SWidget > ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;
};