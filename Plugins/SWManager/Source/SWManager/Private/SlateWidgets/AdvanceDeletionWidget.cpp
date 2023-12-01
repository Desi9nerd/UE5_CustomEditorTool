#include "SlateWidgets/AdvanceDeletionWidget.h"

void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromString(InArgs._TestString))
	];

}