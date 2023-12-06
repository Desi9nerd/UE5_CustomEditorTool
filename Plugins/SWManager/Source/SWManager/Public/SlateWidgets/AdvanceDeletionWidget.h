#pragma once
#include "Widgets/SCompoundWidget.h"

/** Slate Widget을 위해 빈 클래스가 필요
 * 
 */
class SAdvanceDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab) {}

	SLATE_ARGUMENT(TArray< TSharedPtr <FAssetData> >, AssetsDataToStore)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

private:
	TArray< TSharedPtr <FAssetData> > StoredAssetsData;
	TArray<TSharedPtr<FAssetData>> DisplayedAssetsData; // SListView의 source item
	TArray<TSharedRef<SCheckBox>> CheckBoxesArray; // 체크박스들을 담는 배열
	TArray<TSharedPtr<FAssetData>> AssetsDataToDeleteArray; // 체크박스 클릭 시 선택된 에셋들을 기록하는 TArray변수

	void OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData); // 위젯 행 클릭 시
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetListView(); // 에셋 리스트 생성
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	void RefreshAssetListView(); // 에셋 리스트 새로고침

#pragma region ComboBoxForListingCondition

	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructComboBox(); // ComboBox 생성

	TArray<TSharedPtr<FString>> ComboBoxSourceItems;

	TSharedRef<SWidget> OnGenerateComboContent(TSharedPtr<FString> SourceItem);

	void OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo); // ComboBox 선택 시 변경

	TSharedPtr<STextBlock> ComboDisplayTextBlock;

#pragma endregion

#pragma region RowWidgetForAssetListView

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay); // 체크박스 생성
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData); // 체크박스 선택 시 변경 

	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse); // 한줄에 TextContent 띄우기
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay); // 버튼 생성
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData); // 제거버튼 클릭

#pragma endregion

#pragma region TabButtons

	TSharedRef<SButton> ConstructDeleteAllButton(); // 모두 제거 버튼 생성
	TSharedRef<SButton> ConstructSelectAllButton(); // 모두 선택 버튼 생성
	TSharedRef<SButton> ConstructDeselectAllButton(); // 모두 선택해제 버튼 생성

	FReply OnDeleteAllButtonClicked(); // 모두 제거 버튼 클릭
	FReply OnSelectAllButtonClicked(); // 모두 선택 버튼 클릭
	FReply OnDeselectAllButtonClicked(); // 모두 선택해제 버튼 클릭

	TSharedRef<STextBlock> ConstructTextForTabButtons(const FString& TextContent);

#pragma endregion
	
	FSlateFontInfo GetEmboseedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); } // 글꼴 설정
};
