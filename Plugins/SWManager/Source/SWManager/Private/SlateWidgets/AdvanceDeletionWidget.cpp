#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"
#include "SWManager.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	StoredAssetsData = InArgs._AssetsDataToStore; // Slate Widget이 생성(Construct)될 때 변수에 InArgs._AssetsDataArray 넣음으로써 에셋 데이터를 Slate Widget 안에 담는다

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); // 글꼴
	TitleTextFont.Size = 20; // 글자 크기

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
				ConstructAssetListView()
			]
		]

		// 4번째 slot - 3 buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			// Button1 slot
			+ SHorizontalBox::Slot()
			.FillWidth(10.f)
			.Padding(5.f)
			[
				ConstructDeleteAllButton() // 모두 제거 버튼 생성
			]

			// Button2 slot
			+ SHorizontalBox::Slot()
			.FillWidth(10.f)
			.Padding(5.f)
			[
				ConstructSelectAllButton() // 모두 선택 버튼 생성
			]

			// Button3 slot
			+ SHorizontalBox::Slot()
			.FillWidth(10.f)
			.Padding(5.f)
			[
				ConstructDeselectAllButton() // 모두 선택해제 버튼 생성
			]
		]
	];

}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetListView() // 에셋 리스트 생성
{
	// Construct 시 StoredAssetsData에 담은 정보를 사용해서 ConstructedAssetListView 설정
	ConstructedAssetListView = 
		SNew(SListView<TSharedPtr<FAssetData>>)
		.ItemHeight(24.f)
		.ListItemsSource(&StoredAssetsData)
		.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList);

	// TSharedPtr형태인 ConstructedAssetListView를 .ToSharedRef()사용하여 TSharedRef형태로 변환해서 리턴 
	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView() // 에셋 리스트 새로고침
{
	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}

#pragma region RowWidgetForAssetListView

// 매개변수 AssetDataToDisplay로 TArray< TSharedPtr <FAssetData> > StoredAssetsData배열변수의 원소 하나하나 들어온다
TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (false == AssetDataToDisplay.IsValid()) return SNew(STableRow < TSharedPtr <FAssetData> >, OwnerTable); // TSharedRef는 nullptr이 불가능하므로 빈 STableRow를 위와 같은 방식으로 리턴 

	const FString DisplayAssetClassName = AssetDataToDisplay->AssetClass.ToString(); // 에셋 클래스의 이름을 FString 형태로 변수에 저장
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString(); // 에셋의 이름을 FString 형태로 변수에 저장

	FSlateFontInfo AssetClassNameFont = GetEmboseedTextFont();
	AssetClassNameFont.Size = 10;
	FSlateFontInfo AssetNameFont = GetEmboseedTextFont();
	AssetNameFont.Size = 10;

	TSharedRef< STableRow<TSharedPtr<FAssetData>> > ListViewRowWidget =
		SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable).Padding(FMargin(5.f))
		[
			SNew(SHorizontalBox)
			
			// 1번째 slot - 체크 박스
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.FillWidth(0.05f)
			[
				ConstructCheckBox(AssetDataToDisplay) // 체크박스 생성
			]

			// 2번째 slot - 에셋 클래스 이름 띄우기
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			.FillWidth(0.5f)
			[
				ConstructTextForRowWidget(DisplayAssetClassName, AssetClassNameFont) // 에셋 클래스 이름 띄우기
			]

			// 3번째 slot - 에셋 이름 띄우기
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			[
				ConstructTextForRowWidget(DisplayAssetName, AssetNameFont) // 에셋 이름 띄우기
			]

			// 4번째 slot - 버튼
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Fill)
			[
				ConstructButtonForRowWidget(AssetDataToDisplay)
			]
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

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" 이 unchecked 됨"), FColor::Red);

		break;

	case ECheckBoxState::Checked:

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" 이 checked 됨"), FColor::Green);

		break;

	case ECheckBoxState::Undetermined:
		break;

	default:
		break;
	}
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse)
{
	// TextContent를 FontToUse글꼴로 흰색으로 글자 출력
	TSharedRef<STextBlock> ConstructedTextBlock = 
		SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontToUse)
		.ColorAndOpacity(FColor::White);

	return ConstructedTextBlock;
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay) // 버튼 생성
{
	TSharedRef<SButton> ConstructedButton = 
		SNew(SButton)
		.Text(FText::FromString(TEXT("제거")))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteButtonClicked, AssetDataToDisplay); // AssetDataToDisplay 클릭 시 OnDeleteButtonClicked 함수 호출

	return ConstructedButton;
}

FReply SAdvanceDeletionTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData) // 제거버튼 클릭
{
	FSWManagerModule& SWManagerModule = FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	const bool bAssetDeleted = SWManagerModule.DeleteSingleAssetForAssetList(*ClickedAssetData.Get());

	if (bAssetDeleted)
	{
		// List source items 업데이트
		if (StoredAssetsData.Contains(ClickedAssetData))
		{
			StoredAssetsData.Remove(ClickedAssetData); // 클릭한 에셋을 StoredAssetsData배열에서 제거
		}

		// 에셋 리스트 새로고침
		RefreshAssetListView();
	}

	
	return FReply::Handled();
}

#pragma endregion

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteAllButton() // 모두 제거 버튼 생성
{
	TSharedRef<SButton> DeleteAllButton = 
		SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteAllButtonClicked);

	DeleteAllButton->SetContent(ConstructTextForTabButtons(TEXT("모두 제거")));

	return DeleteAllButton;
}

FReply SAdvanceDeletionTab::OnDeleteAllButtonClicked() // 모두 제거 버튼 클릭
{
	DebugHeader::Print(TEXT("버튼이 눌린 모든 에셋을 제거"), FColor::Cyan);
	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructSelectAllButton() // 모두 선택 버튼 생성
{
	TSharedRef<SButton> SelectAllButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnSelectAllButtonClicked);

	SelectAllButton->SetContent(ConstructTextForTabButtons(TEXT("모두 선택")));

	return SelectAllButton;
}

FReply SAdvanceDeletionTab::OnSelectAllButtonClicked() // 모두 선택 버튼 클릭
{
	DebugHeader::Print(TEXT("버튼이 눌린 에셋들 모두 선택"), FColor::Cyan);
	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeselectAllButton() // 모두 선택해제 버튼 생성
{
	TSharedRef<SButton> DeselectAllButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeselectAllButtonClicked);

	DeselectAllButton->SetContent(ConstructTextForTabButtons(TEXT("모두 선택해제")));

	return DeselectAllButton;
}

FReply SAdvanceDeletionTab::OnDeselectAllButtonClicked() // 모두 선택해제 버튼 클릭
{
	DebugHeader::Print(TEXT("버튼이 눌린 에셋들 모두 선택해제"), FColor::Cyan);
	return FReply::Handled();
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForTabButtons(const FString& TextContent)
{
	FSlateFontInfo ButtonTextFont = GetEmboseedTextFont();
	ButtonTextFont.Size = 10;

	TSharedRef<STextBlock> ConstructedTextBlock = 
		SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(ButtonTextFont)
		.Justification(ETextJustify::Center);

	return ConstructedTextBlock;
}
