#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/SWManagerStyle.h"
#include "ActorTreeItem.h"
#include "SWManager.h"

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

const TSharedRef<SWidget> FOutlinerSelectionColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) // 각 Row Widget에 CheckBox 생성
{
	TObjectPtr<FActorTreeItem> ActorTreeItem = TreeItem->CastTo<FActorTreeItem>();

	if (!ActorTreeItem || false == ActorTreeItem->IsValid()) // Tree Item이 nullptr이거나 Level에 없으면
		return SNullWidget::NullWidget;


	FSWManagerModule& SWManagerModule = FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	const bool bIsActorSelectionLocked = SWManagerModule.CheckIsActorSelectionLocked(ActorTreeItem->Actor.Get());

	TSharedRef<SCheckBox> ConstructedRowWidgetCheckBox =
		SNew(SCheckBox)
		.Visibility(EVisibility::Visible)
		.HAlign(HAlign_Center)
		.IsChecked(bIsActorSelectionLocked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked) // bIsActorSelectionLocked의 true/false여부로 CheckBox의 Checked/Unchecked 결정
		.OnCheckStateChanged(this, &FOutlinerSelectionColumn::OnRowWidgetCheckStateChanged, ActorTreeItem->Actor); // CheckBox의 상태변경 시 OnRowWdigetCheckStateChanged 호출

	return ConstructedRowWidgetCheckBox;
}

void FOutlinerSelectionColumn::OnRowWidgetCheckStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> CorrespondingActor)
{
	FSWManagerModule& SWManagerModule = FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	switch (NewState)
	{
	case ECheckBoxState::Unchecked:

		SWManagerModule.ProcessLockingForOutliner(CorrespondingActor.Get(), false);
		break;

	case ECheckBoxState::Checked:

		SWManagerModule.ProcessLockingForOutliner(CorrespondingActor.Get(), true);
		break;

	case ECheckBoxState::Undetermined:
		break;

	default:
		break;
	}
}
