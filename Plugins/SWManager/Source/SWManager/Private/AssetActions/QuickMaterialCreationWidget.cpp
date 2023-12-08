#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"

#pragma region QuickMaterialCreationCore

void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	if (bCustomMaterialName)
	{
		// 이름을 입력하지 않거나 "M_"만 입력했다면
		if (MaterialName.IsEmpty() || MaterialName.Equals(TEXT("M_")))
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("해당 이름을 사용할 수 없습니다."));
			return;
		}
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<TObjectPtr<UTexture2D>> SelectedTexturesArray;
	FString SelectedTextureFolderPath;
	uint32 PinsConnectedCounter = 0;

	// 선택한 데이터를 처리
	if (false == ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, SelectedTextureFolderPath)) return;

	// 생성하려는 이름(=MaterialName)이 이미 있는지 확인하고 이미 있다면 return
	if (CheckIsNameUsed(SelectedTextureFolderPath, MaterialName)) return;

	TObjectPtr<UMaterial> CreatedMaterial = CreateMaterialAsset(MaterialName, SelectedTextureFolderPath); // Material 생성 후 CreatedMaterial변수에 담음

	if (false == IsValid(CreatedMaterial)) // Material 생성에 실패했다면
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Material 생성에 실패했습니다."));
		return;
	}

	// 선택한 Texture가 해당 Texture에 알맞는 노드 생성 후 루프를 돌며 알맞은 소켓을 찾아 핀 연결
	for (TObjectPtr<UTexture2D> SelectedTexture : SelectedTexturesArray)
	{
		if (false == IsValid(SelectedTexture)) continue;

		Default_CreateMaterialNodes(CreatedMaterial, SelectedTexture, PinsConnectedCounter); // Texture타입에 알맞는 노드 생성 후 핀 연결
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

bool UQuickMaterialCreationWidget::CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck) // 생성하려는 이름이 이미 있는지 확인
{
	TArray<FString> ExistingAssetsPaths = UEditorAssetLibrary::ListAssets(FolderPathToCheck, false); // 에셋들의 파일경로를 담음

	for (const FString& ExistingAssetPath : ExistingAssetsPaths)
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(ExistingAssetPath); // 파일경로에 있는 에셋 이름을 담음 

		// ExistingAssetName이 새로 생성하려는 이름(=MaterialNameToCheck)과 같다면 메시지를 띄우고 true 리턴 종료
		if (ExistingAssetName.Equals(MaterialNameToCheck)) 
		{
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, MaterialNameToCheck +
				TEXT(" 는 이미 사용중인 이름입니다."));

			return true;
		}
	}

	return false;
}

TObjectPtr<UMaterial> UQuickMaterialCreationWidget::CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial) // Material 생성
{
	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools")); // FAssetToolsModule를 사용하려면 #include "AssetToolsModule.h" 필요

	TObjectPtr<UMaterialFactoryNew> MaterialFactory = NewObject<UMaterialFactoryNew>(); // UMaterialFactoryNew를 사용하려면 #include "Factories/MaterialFactoryNew.h" 필요

	TObjectPtr<UObject> CreatedObject = AssetToolsModule.Get().CreateAsset(NameOfTheMaterial, PathToPutMaterial, UMaterial::StaticClass(), MaterialFactory); // NameOfTheMaterial이름으로 에셋 생성

	return Cast<UMaterial>(CreatedObject); // 생성한 에셋을 UMaterial타입으로 캐스팅하여 UMaterial타입으로 return
}

void UQuickMaterialCreationWidget::Default_CreateMaterialNodes(TObjectPtr<UMaterial> CreatedMaterial, TObjectPtr<UTexture2D> SelectedTexture, uint32& PinsConnectedCounter) // Material Node 생성
{
	TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode = NewObject<UMaterialExpressionTextureSample>(CreatedMaterial);

	if (false == IsValid(TextureSampleNode)) return;

	if (false == CreatedMaterial->BaseColor.IsConnected()) // BaseColor가 연결X
	{
		// TextureSampleNode 노드 연결
		if (TryConnectBaseColor(TextureSampleNode, SelectedTexture, CreatedMaterial))
		{
			PinsConnectedCounter++;
			return;
		}
	}
}

#pragma endregion

#pragma region CreateMaterialNodes

bool UQuickMaterialCreationWidget::TryConnectBaseColor(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial) 
{
	for (const FString& BaseColorName : BaseColorArray)
	{
		if (SelectedTexture->GetName().Contains(BaseColorName))
		{
			// 노드를 BaseColor 소켓에 핀 연결
			TextureSampleNode->Texture = SelectedTexture;

			CreatedMaterial->Expressions.Add(TextureSampleNode);
			CreatedMaterial->BaseColor.Expression = TextureSampleNode;
			CreatedMaterial->PostEditChange();

			TextureSampleNode->MaterialExpressionEditorX -= 600; // 위치가 겹치지 않게 위치 이동

			return true;
		}
	}

	return false;
}

#pragma endregion