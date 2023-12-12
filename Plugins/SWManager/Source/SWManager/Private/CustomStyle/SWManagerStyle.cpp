#include "CustomStyle/SWManagerStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"

FName FSWManagerStyle::StyleSetName = FName("SWManagerStyle"); // 이름 설정
TSharedPtr<FSlateStyleSet> FSWManagerStyle::CreatedSlateStyleSet = nullptr;

void FSWManagerStyle::InitializeIcons()
{
	if (false == CreatedSlateStyleSet.IsValid()) // CreatedSlateStyleSet이 nullptr이면(=설정되지 않았다면)
	{
		CreatedSlateStyleSet = CreateSlateStyleSet(); // SlateStyleSet을 생성한 후 변수에 담는다
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet); // SlateStyleSet를 등록
	}
}

TSharedRef<FSlateStyleSet> FSWManagerStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	const FString IconDirectory = IPluginManager::Get().FindPlugin(TEXT("SWManager"))->GetBaseDir() / "Resources"; // Icon이미지가 저장된 폴더 위치를 FString형태로 담기

	CustomStyleSet->SetContentRoot(IconDirectory); // ContentRoot를 Icon이미지가 저장된 폴더 위치로 설정

	const FVector2D Icon16x16(16.f, 16.f); // Icon 사이즈로 사용할 변수

	CustomStyleSet->Set("ContentBrowser.DeleteUnusedAssets", 	new FSlateImageBrush(IconDirectory / "DeleteUnusedAsset.png", Icon16x16)); // PropertyName을 "ContentBrowser.DeleteUnusedAssets"로 설정하고 "DeleteUnusedAsset.png"이름의 Icon이미지를 Icon16x16 사이즈로 담는다

	CustomStyleSet->Set("ContentBrowser.DeleteEmptyFolders",
		new FSlateImageBrush(IconDirectory / "DeleteEmptyFolders.png", Icon16x16));

	CustomStyleSet->Set("ContentBrowser.AdvanceDeletion",
		new FSlateImageBrush(IconDirectory / "AdvanceDeletion.png", Icon16x16));

	CustomStyleSet->Set("LevelEditor.LockSelection",
		new FSlateImageBrush(IconDirectory / "SelectionLock.png", Icon16x16));

	CustomStyleSet->Set("LevelEditor.UnlockSelection",
		new FSlateImageBrush(IconDirectory / "SelectionUnlock.png", Icon16x16));

	return CustomStyleSet;
}

void FSWManagerStyle::ShutDown()
{
	if (CreatedSlateStyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*CreatedSlateStyleSet); // 참조 해제
		CreatedSlateStyleSet.Reset();
	}
}