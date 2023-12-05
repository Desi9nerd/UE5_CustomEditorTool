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

	TSharedRef<SListView<TSharedPtr<FAssetData>>> ConstructAssetListView(); // ���� ����Ʈ ����
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;
	void RefreshAssetListView(); // ���� ����Ʈ ���ΰ�ħ

#pragma region RowWidgetForAssetListView

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay); // üũ�ڽ� ����
	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData); // üũ�ڽ� ���� �� ���� 

	TSharedRef<STextBlock> ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse); // ���ٿ� TextContent ����
	TSharedRef<SButton> ConstructButtonForRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay); // ��ư ����
	FReply OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData); // ���Ź�ư Ŭ��

#pragma endregion

	FSlateFontInfo GetEmboseedTextFont() const { return FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); } // �۲� ����
};
