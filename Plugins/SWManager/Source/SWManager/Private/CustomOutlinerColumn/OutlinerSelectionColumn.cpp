#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/SWManagerStyle.h"

SHeaderRow::FColumn::FArguments FOutlinerSelectionColumn::ConstructHeaderRowColumn()
{
	SHeaderRow::FColumn::FArguments ConstructedHeaderRow =
		SHeaderRow::Column(GetColumnID())
		.FixedWidth(24.f)
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HAlignCell(HAlign_Center)
		.VAlignCell(VAlign_Center)
		.DefaultTooltip(FText::FromString(TEXT("선택한 Actor LOCK - 아이콘을 눌러 선택한 액터 잠금"))) // 마우스를 올렸을때 나오는 문구
		[
			SNew(SImage)
			.ColorAndOpacity(FSlateColor::UseForeground())
			.Image(FSWManagerStyle::GetCreatedSlateStyleSet()->GetBrush(FName("LevelEditor.LockSelection"))) // SWManagerStyle.h에서 만든 SlateStyleSet 사용. "LevelEditor.LockSelection"이름의 Style 사용.
		];

	return ConstructedHeaderRow;
}

const TSharedRef<SWidget> FOutlinerSelectionColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row)
{
	return SNullWidget::NullWidget; // TSharedRef<SWidget>의 nullptr버젼
}