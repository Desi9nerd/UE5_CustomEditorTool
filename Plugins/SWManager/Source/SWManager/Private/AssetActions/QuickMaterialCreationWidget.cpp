#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"

#pragma region QuickMaterialCreationCore

void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	if (bCustomMaterialName)
	{
		// 이름을 입력하지 않거나 이미 있는 이름을 입력했다면
		if (MaterialName.IsEmpty() || MaterialName.Equals(TEXT("M_"))) 
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("해당 이름을 사용할 수 없습니다."));
			return;
		}
	}
}

#pragma endregion