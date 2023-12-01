#include "SlateWidgets/AdvanceDeletionWidget.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText")); // 글꼴
	TitleTextFont.Size = 30; // 글자 크기

	ChildSlot
	[		
		SNew(SVerticalBox) // Main vertical box

		// First vertical slot for title text
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("제거 마법사")))
			.Font(TitleTextFont) // 글꼴, 글자 크기 설정
			.Justification(ETextJustify::Center) // 글자 중앙 정렬
			.ColorAndOpacity(FColor::White)
		]


	];

}