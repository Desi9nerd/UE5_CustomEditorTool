#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"

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
		.AutoHeight()
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
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString(); // 에셋의 이름을 FString 형태로 변수에 저장

	TSharedRef< STableRow<TSharedPtr<FAssetData>> > ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			SNew(STextBlock)
			.Text(FText::FromString(DisplayAssetName)) // FString을 FText형태로 변환 
		];

	return ListViewRowWidget;
}
