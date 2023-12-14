#include "CustomStyle/SWManagerStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/StyleColors.h"

FName FSWManagerStyle::StyleSetName = FName("SWManagerStyle"); // �̸� ����
TSharedPtr<FSlateStyleSet> FSWManagerStyle::CreatedSlateStyleSet = nullptr;

void FSWManagerStyle::InitializeIcons()
{
	if (false == CreatedSlateStyleSet.IsValid()) // CreatedSlateStyleSet�� nullptr�̸�(=�������� �ʾҴٸ�)
	{
		CreatedSlateStyleSet = CreateSlateStyleSet(); // SlateStyleSet�� ������ �� ������ ��´�
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet); // SlateStyleSet�� ���
	}
}

TSharedRef<FSlateStyleSet> FSWManagerStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	const FString IconDirectory = IPluginManager::Get().FindPlugin(TEXT("SWManager"))->GetBaseDir() / "Resources"; // Icon�̹����� ����� ���� ��ġ�� FString���·� ���

	CustomStyleSet->SetContentRoot(IconDirectory); // ContentRoot�� Icon�̹����� ����� ���� ��ġ�� ����

	const FVector2D Icon16x16(16.f, 16.f); // Icon ������� ����� ����

	CustomStyleSet->Set("ContentBrowser.DeleteUnusedAssets", 	new FSlateImageBrush(IconDirectory / "DeleteUnusedAsset.png", Icon16x16)); // PropertyName�� "ContentBrowser.DeleteUnusedAssets"�� �����ϰ� "DeleteUnusedAsset.png"�̸��� Icon�̹����� Icon16x16 ������� ��´�

	CustomStyleSet->Set("ContentBrowser.DeleteEmptyFolders",
		new FSlateImageBrush(IconDirectory / "DeleteEmptyFolders.png", Icon16x16));

	CustomStyleSet->Set("ContentBrowser.AdvanceDeletion",
		new FSlateImageBrush(IconDirectory / "AdvanceDeletion.png", Icon16x16));

	CustomStyleSet->Set("LevelEditor.LockSelection",
		new FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16));

	CustomStyleSet->Set("LevelEditor.UnlockSelection",
		new FSlateImageBrush(IconDirectory / "SelectionUnlock.png", Icon16x16));

	// World Outliner�� �ߴ� CheckBox(=ToggleButton) �̹��� ����� ���� FCheckBoxStyle ���� �� ������ ����
	const FCheckBoxStyle SelectionLockToggleButtonStyle = 
		FCheckBoxStyle()
		.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
		.SetPadding(FMargin(10.f))

		// Uncheck images
		.SetUncheckedImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::White25))
		.SetUncheckedHoveredImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::AccentBlue))
		.SetUncheckedPressedImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::Foreground))

		// Checked images
		.SetCheckedImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::Foreground))
		.SetCheckedHoveredImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::AccentBlack))
		.SetCheckedPressedImage(FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16, FStyleColors::AccentGray));

	CustomStyleSet->Set("SceneOutliner.SelectionLock", SelectionLockToggleButtonStyle);


	return CustomStyleSet;
}

void FSWManagerStyle::ShutDown()
{
	if (CreatedSlateStyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*CreatedSlateStyleSet); // ���� ����
		CreatedSlateStyleSet.Reset();
	}
}