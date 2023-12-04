#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	StoredAssetsData = InArgs._AssetsDataToStore; // Slate Widget이 생성(Construct)될 때 변수에 InArgs._AssetsDataArray 넣음으로써 에셋 데이터를 Slate Widget 안에 담는다

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); // 글꼴
	TitleTextFont.Size = 30; // 글자 크기

	ChildSlot
	[		
		SNew(SVerticalBox) // Main vertical box

		// First Vertical slot - Title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("제거 마법사")))
			.Font(TitleTextFont) // 글꼴, 글자 크기 설정
			.Justification(ETextJustify::Center) // 글자 중앙 정렬
			.ColorAndOpacity(FColor::White)
		]

		// 2번째 Slot - drop down to specify the listing condition and help text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]

		// 3번째 slot - 에셋 리스트
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill) // Scroll 가능하도록 설정
		[
			SNew(SScrollBox) // 사용하려면 #include "SlateBasics.h" 필요
			+ SScrollBox::Slot()
			[
				SNew(SListView<TSharedPtr<FAssetData>>)
				.ItemHeight(24.0f)
				.ListItemsSource(&StoredAssetsData)
				.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
			]
		]

		// 4번째 slot - 3 buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]
	];

}

// 매개변수 AssetDataToDisplay로 TArray< TSharedPtr <FAssetData> > StoredAssetsData배열변수의 원소 하나하나 들어온다
TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (false == AssetDataToDisplay.IsValid()) return SNew(STableRow < TSharedPtr <FAssetData> >, OwnerTable); // TSharedRef는 nullptr이 불가능하므로 빈 STableRow를 위와 같은 방식으로 리턴 

	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString(); // 에셋의 이름을 FString 형태로 변수에 저장

	TSharedRef< STableRow<TSharedPtr<FAssetData>> > ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(SHorizontalBox)
			
			// 1번째 slot - 체크 박스
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(.05f)
			[
				ConstructCheckBox(AssetDataToDisplay) // 체크박스 생성
			]

			// 2번째 slot - 에셋 클래스 이름 띄우기

			// 3번째 slot - 에셋 이름 띄우기
			+SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(DisplayAssetName)) // 에셋 이름
			]

			// 4번째 slot - 버튼
		];

	return ListViewRowWidget;
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay) // 체크박스 생성
{
	TSharedRef<SCheckBox> ConstructedCheckBox = 
		SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);

	return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData) // 체크박스 선택 시 변경
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is unchecked"), FColor::Red);

		break;

	case ECheckBoxState::Checked:

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is checked"), FColor::Green);

		break;

	case ECheckBoxState::Undetermined:
		break;

	default:
		break;
	}
}
