#pragma once
#include "Widgets/SCompoundWidget.h"

/** Slate Widget�� ���� �� Ŭ������ �ʿ�
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
	TArray<TSharedPtr<FAssetData>> DisplayedAssetsData; // SListView�� source item
	TArray<TSharedRef<SCheckBox>> CheckBoxesArray; // üũ�ڽ����� ��� �迭
	TArray<TSharedPtr<FAssetData>> AssetsDataToDeleteArray; // üũ�ڽ� Ŭ�� �� ���õ� ���µ��� ����ϴ� TArray����

	void OnRowWidgetMouseButtonClicked(TSharedPtr<FAssetData> ClickedData); // ���� �� Ŭ�� ��
	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetListView(); // ���� ����Ʈ ����
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	void RefreshAssetListView(); // ���� ����Ʈ ���ΰ�ħ

#pragma region ComboBoxForListingCondition

	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructComboBox(); // ComboBox ����

	TArray<TSharedPtr<FString>> ComboBoxSourceItems;

	TSharedRef<SWidget> OnGenerateComboContent(TSharedPtr<FString> SourceItem);

	void OnComboSelectionChanged(TSharedPtr<FString> SelectedOption, ESelectInfo::Type InSelectInfo); // ComboBox ���� �� ����

	TSharedPtr<STextBlock> ComboDisplayTextBlock;

#pragma endregion

#pragma region RowWidgetForAssetListView

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay); // üũ�ڽ� ����
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData); // üũ�ڽ� ���� �� ���� 

	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse); // ���ٿ� TextContent ����
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay); // ��ư ����
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData); // ���Ź�ư Ŭ��

#pragma endregion

#pragma region TabButtons

	TSharedRef<SButton> ConstructDeleteAllButton(); // ��� ���� ��ư ����
	TSharedRef<SButton> ConstructSelectAllButton(); // ��� ���� ��ư ����
	TSharedRef<SButton> ConstructDeselectAllButton(); // ��� �������� ��ư ����

	FReply OnDeleteAllButtonClicked(); // ��� ���� ��ư Ŭ��
	FReply OnSelectAllButtonClicked(); // ��� ���� ��ư Ŭ��
	FReply OnDeselectAllButtonClicked(); // ��� �������� ��ư Ŭ��

	TSharedRef<STextBlock> ConstructTextForTabButtons(const FString& TextContent);

#pragma endregion
	
	FSlateFontInfo GetEmboseedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); } // �۲� ����
};
