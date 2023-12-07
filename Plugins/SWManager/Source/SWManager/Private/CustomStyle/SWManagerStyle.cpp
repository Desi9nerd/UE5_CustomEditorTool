#include "CustomStyle/SWManagerStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

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

	CustomStyleSet->Set("ContentBrowser.DeleteUnusedAssets", 	new FSlateImageBrush(IconDirectory / "DeleteUnusedAsset.png", Icon16x16)); // "DeleteUnusedAsset.png"�̸��� Icon�̹����� Icon16x16 ������� �ҷ��´�

	return CustomStyleSet;
}

void FSWManagerStyle::ShutDown()
{
}