#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"
#include "SWManager.h"

#define ListAll TEXT("모든 에셋")
#define ListUnused TEXT("사용하지 않는 에셋")
#define ListSameName TEXT("같은 이름을 가진 에셋 ")

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	StoredAssetsData = InArgs._AssetsDataToStore; // Slate Widget이 생성(Construct)될 때 변수에 InArgs._AssetsDataArray 넣음으로써 에셋 데이터를 Slate Widget 안에 담는다
	DisplayedAssetsData = StoredAssetsData; // Slate Widget 생성 시에는 StoredAssetData와 같다

	CheckBoxesArray.Empty();
	AssetsDataToDeleteArray.Empty();
	ComboBoxSourceItems.Empty();

	ComboBoxSourceItems.Add(MakeShared<FString>(ListAll)); // ComboBox에 ListAll 추가
	ComboBoxSourceItems.Add(MakeShared<FString>(ListUnused)); // ComboBox에 ListUnused 추가
	ComboBoxSourceItems.Add(MakeShared<FString>(ListSameName)); // ComboBox에 ListSameName 추가

	FSlateFontInfo TitleTextFont = GetEmboseedTextFont(); // 글꼴
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

			// ComboBox slot
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				ConstructComboBox()
			]

			// ComboBox slot 도움말
			+ SHorizontalBox::Slot()
			.FillWidth(0.6f)
			[
				ConstructComboHelpTexts(TEXT("좌측 상단 Drop down에서 리스트 옵션을 지정할 수 있습니다. 왼쪽 마우스를 클릭하면 Asset이 있는 곳으로 이동합니다."),
					ETextJustify::Center)
			]

			// Folder path 도움말
			+ SHorizontalBox::Slot()
			.FillWidth(0.1f)
			[
				ConstructComboHelpTexts(TEXT("현재 폴더:\n") + InArgs._CurrentSelectedFolder, ETextJustify::Right)
			]
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

void SAdvanceDeletionTab::OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData) // 위젯 행 클릭 시
{
	FSWManagerModule& SWManagerModule = FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	SWManagerModule.SyncCBToClickedAssetForAssetList(ClickedData->ObjectPath.ToString()); // 위젯 행 클릭 시 콘텐츠 브라우저에서 해당 에셋이 활성화된다 
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::ConstructAssetListView() // 에셋 리스트 생성
{
	// Construct 시 DisplayedAssetsData에 담은 정보를 사용해서 ConstructedAssetListView 설정
	ConstructedAssetListView = 
		SNew(SListView<TSharedPtr<FAssetData>>)
		.ItemHeight(24.f)
		.ListItemsSource(&DisplayedAssetsData)
		.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList) // 행에 에셋 생성
		.OnMouseButtonClick(this, &SAdvanceDeletionTab::OnRowWidgetMouseButtonClicked); // 해당 행을 클릭 시 함수 실행

	// TSharedPtr형태인 ConstructedAssetListView를 .ToSharedRef()사용하여 TSharedRef형태로 변환해서 리턴 
	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView() // 에셋 리스트 새로고침
{
	CheckBoxesArray.Empty();
	AssetsDataToDeleteArray.Empty();

	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}

#pragma region ComboBoxForListingCondition

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTab::ConstructComboBox() // ComboBox 생성
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox =
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboBoxSourceItems)
		.OnGenerateWidget(this, &SAdvanceDeletionTab::OnGenerateComboContent)
		.OnSelectionChanged(this, &SAdvanceDeletionTab::OnComboSelectionChanged) // 선택 시 함수 실행
		[
			SAssignNew(ComboDisplayTextBlock, STextBlock)
			.Text(FText::FromString(TEXT("에셋 리스트 보여주기 옵션")))
		];

	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvanceDeletionTab::OnGenerateComboContent(TSharedPtr<FString> SourceItem)
{
	TSharedRef<STextBlock> ContructedComboText = 
		SNew(STextBlock)
		.Text(FText::FromString(*SourceItem.Get()));

	return ContructedComboText;
}

void SAdvanceDeletionTab::OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo) // ComboBox 선택 시 변경
{
	DebugHeader::Print(*SelectedOption.Get(), FColor::Cyan);

	ComboDisplayTextBlock->SetText(FText::FromString(*SelectedOption.Get())); // 매개변수로 들어온 SelectedOption 문구를 띄운다

	FSWManagerModule& SWManagerModule = FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	// 선택된 옵션(ListAll or ListUnused)에 따라 필터링된 에셋 데이터를 SWManagerModule에 넘긴다
	if (*SelectedOption.Get() == ListAll) // 모든 에셋 리스트
	{
		DisplayedAssetsData = StoredAssetsData; // 모든 StoredAssetsData을 DisplayedAssetsData에 담아 위젯에 띄운다
		RefreshAssetListView();
	}
	else if (*SelectedOption.Get() == ListUnused) // 사용하지 않는 모든 에셋 리스트
	{
		SWManagerModule.ListUnusedAssetsForAssetList(StoredAssetsData, DisplayedAssetsData); // StoredAssetData의 에셋들 중 참조된 경우를 검사하여 사용하지 않는 에셋들을 DisplayedAssetsData에 업데이트한다.
		RefreshAssetListView();
	}
	else if (*SelectedOption.Get() == ListSameName) // 같은 이름을 사용하는 모든 에셋 리스트
	{
		SWManagerModule.ListSameNameAssetsForAssetList(StoredAssetsData, DisplayedAssetsData); // StoredAssetData의 에셋들 중 같은 이름을 가진 에셋들을 DisplayedAssetsData에 업데이트한다.
		RefreshAssetListView();
	}
}

#pragma endregion

#pragma region RowWidgetForAssetListView

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructComboHelpTexts(const FString& TextContent, ETextJustify::Type TextJustify)
{
	TSharedRef<STextBlock> ConstructedHelpText =
		SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Justification(TextJustify)
		.AutoWrapText(true);

	return ConstructedHelpText;
}

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

	CheckBoxesArray.Add(ConstructedCheckBox); // 체크박스들을 담는 배열에 생성한 체크박스 추가

	return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData) // 체크박스 선택 시 변경
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		if (AssetsDataToDeleteArray.Contains(AssetData))
		{
			AssetsDataToDeleteArray.Remove(AssetData); // 선택된 에셋 목록에서 빼줌
		}
		break;

	case ECheckBoxState::Checked:
		AssetsDataToDeleteArray.AddUnique(AssetData);  // 선택된 에셋 목록에서 추가
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
		 if(DisplayedAssetsData.Contains(ClickedAssetData))
		 {
			DisplayedAssetsData.Remove(ClickedAssetData); // 클릭한 에셋을 DisplayedAssetsData배열에서 제거
		 }

		// 에셋 리스트 새로고침
		RefreshAssetListView();
	}

	
	return FReply::Handled();
}

#pragma endregion

#pragma region TabButtons

TSharedRef<SButton> SAdvanceDeletionTab::ConstructDeleteAllButton() // 모두 제거 버튼 생성
{
	TSharedRef<SButton> DeleteAllButton = 
		SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteAllButtonClicked);

	DeleteAllButton->SetContent(ConstructTextForTabButtons(TEXT("선택한 에셋 모두 제거")));

	return DeleteAllButton;
}

FReply SAdvanceDeletionTab::OnDeleteAllButtonClicked() // 모두 제거 버튼 클릭
{
	if (AssetsDataToDeleteArray.Num() == 0) // 선택한 에셋이 없는 경우
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("현재 선택된 에셋이 없습니다."));
		return FReply::Handled();
	}

	TArray<FAssetData> AssetDataToDelete; // 제거할 에셋 목록 TArray배열

	for (const TSharedPtr<FAssetData>& Data : AssetsDataToDeleteArray) // 선택한 에셋들
	{
		AssetDataToDelete.Add(*Data.Get()); // 선택한 에셋들을 제거할 에셋 목록에 담는다
	}

	FSWManagerModule& SWManagerModule =
		FModuleManager::LoadModuleChecked<FSWManagerModule>(TEXT("SWManager"));

	const bool bAssetsDeleted = SWManagerModule.DeleteMultipleAssetsForAssetList(AssetDataToDelete); // SWManger클래스의 DeleteMultipleAssetsForAssetList함수를 사용하여 AssetDataToDelete에 담긴 에셋들을 한번에 제거

	if (bAssetsDeleted) // 에셋들이 제거 되었다면
	{
		for (const TSharedPtr<FAssetData>& DeletedData : AssetsDataToDeleteArray)
		{
			if (StoredAssetsData.Contains(DeletedData))
			{
				StoredAssetsData.Remove(DeletedData); // 제거된 에셋들을 StoredAssetsData에서 빼줌
			}
			if (DisplayedAssetsData.Contains(DeletedData))
			{
				DisplayedAssetsData.Remove(DeletedData); // 제거된 에셋들을 DisplayedAssetsData에서 빼줌
			}
		}

		RefreshAssetListView(); // 에셋 리스트 새로고침
	}

	// 제거를 위해 Module에 데이터를 넘긴다
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
	if (CheckBoxesArray.Num() == 0) return FReply::Handled(); // 체크박스가 없으면 리턴

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (false == CheckBox->IsChecked())
		{
			CheckBox->ToggleCheckedState();
		}
	}

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
	if (CheckBoxesArray.Num() == 0) return FReply::Handled(); // 체크박스가 없으면 리턴

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxesArray)
	{
		if (CheckBox->IsChecked())
		{
			CheckBox->ToggleCheckedState();
		}
	}

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

#pragma endregion