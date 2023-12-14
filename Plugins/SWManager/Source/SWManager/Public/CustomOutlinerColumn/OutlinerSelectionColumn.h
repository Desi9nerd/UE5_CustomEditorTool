#pragma once
#include "ISceneOutlinerColumn.h"

/** Custom World Outliner �����
 *  World Outliner�� Column�� Lock/Unlock ��� �ֱ�
 *  ISceneOutlinerColumn�� ��ӹް� �Լ����� override�Ͽ� �����. ISceneOutlinerColumn�� ��ӹ޾� ����Ϸ��� .Build.cs�� "SceneOutliner" �߰��Ͽ��� ��.
 */
class FOutlinerSelectionLockColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutliner) {}

	virtual FName GetColumnID() override { return FName("SelectionLock"); }
	static FName GetID() { return FName("SelectionLock"); }

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef< SWidget > ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;
};