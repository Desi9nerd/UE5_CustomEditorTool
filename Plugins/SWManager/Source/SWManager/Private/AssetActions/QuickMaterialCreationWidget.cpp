#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"

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

		TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
		TArray<UTexture2D*> SelectedTexturesArray;
		FString SelectedTextureFolderPath;

		// 선택한 데이터를 처리
		if (false == ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, SelectedTextureFolderPath)) return;

		DebugHeader::Print(SelectedTextureFolderPath, FColor::Cyan); // 선택한 데이터의 파일 경로를 디버깅 메시지로 띄움
	}
}

// 선택된 데이터 처리. Texture면 Material로 변환하고 true 리턴, Texture가 아니면 false 리턴
bool UQuickMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProccess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath)
{
	if (SelectedDataToProccess.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("선택된 Texture가 없습니다."));
		return false;
	}

	bool bMaterialNameSet = false;

	for (const FAssetData& SelectedData : SelectedDataToProccess)
	{
		TObjectPtr<UObject> SelectedAsset = SelectedData.GetAsset();

		if (false == IsValid(SelectedAsset)) continue; // 선택된 에셋이 없다면 continue

		TObjectPtr<UTexture2D> SelectedTexture = Cast<UTexture2D>(SelectedAsset); // 선택된 에셋을 Texture 캐스팅
		
		if (false == IsValid(SelectedTexture)) // 선택된 에셋이 Texture타입이 아니라면
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("한 가지 Texture만 선택하십시오.\n") +
				SelectedAsset->GetName() + TEXT(" 은 Texture가 아닙니다."));

			return false;
		}

		OutSelectedTexturesArray.Add(SelectedTexture); // 선택된 에셋이 Texture라면 OutSelectedTexturesArray 담는다

		if (OutSelectedTexturePackagePath.IsEmpty())
		{
			OutSelectedTexturePackagePath = SelectedData.PackagePath.ToString(); // 선택된 에셋(=Texture)의 파일 경로를 담는다.
		}

		if (false == bCustomMaterialName && false == bMaterialNameSet)
		{
			MaterialName = SelectedAsset->GetName(); // 이름을 가져와서 담음
			MaterialName.RemoveFromStart(TEXT("T_")); // 접두어 T_ 지움
			MaterialName.InsertAt(0, TEXT("M_")); // 접두어 M_ 붙임

			bMaterialNameSet = true;
		}
	}

	return true;
}

#pragma endregion
