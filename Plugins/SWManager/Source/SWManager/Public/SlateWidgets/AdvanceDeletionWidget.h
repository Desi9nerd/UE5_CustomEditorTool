#pragma once
#include "Widgets/SCompoundWidget.h"

/** Slate Widget을 위해 빈 클래스가 필요
 * 
 */
class SAdvanceDeletionTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab) {}

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);
};
