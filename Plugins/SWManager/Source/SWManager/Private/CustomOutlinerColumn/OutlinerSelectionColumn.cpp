#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/SWManagerStyle.h"

SHeaderRow::FColumn::FArguments FOutlinerSelectionLockColumn::ConstructHeaderRowColumn()
{
	SHeaderRow::FColumn::FArguments ConstructedHeaderRow =
		SHeaderRow::Column(GetColumnID())
		.FixedWidth(24.f)
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HAlignCell(HAlign_Center)
		.VAlignCell(VAlign_Center)
		.DefaultTooltip(FText::FromString(TEXT("Actor Selection Lock - Press icon to lock actor selection"))) // ���콺�� �÷����� ������ ����
		[
			SNew(SImage)
			.ColorAndOpacity(FSlateColor::UseForeground())
			.Image(FSWManagerStyle::GetCreatedSlateStyleSet()->GetBrush(FName("LevelEditor.LockSelection"))) // SWManagerStyle.h���� ���� SlateStyleSet ���. "LevelEditor.LockSelection"�̸��� Style ���.
		];

	return ConstructedHeaderRow;
}

const TSharedRef<SWidget> FOutlinerSelectionLockColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row)
{
	return SNullWidget::NullWidget; // TSharedRef<SWidget>�� nullptr����
}