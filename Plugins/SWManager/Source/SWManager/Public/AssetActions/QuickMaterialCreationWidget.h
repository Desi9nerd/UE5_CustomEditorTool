#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

/** Material 관련 EditorUtilityWidget
 *  선택한 Texture를 사용해서 Material 생성
 *	bCustomMaterailName==true면 이름을 지을 수 있고, false면 Texture이름에 M_ 접두어를 붙인 이름으로 생성 
 */
UCLASS()
class SWMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:

#pragma region QuickMaterialCreationCore

	UFUNCTION(BlueprintCallable)
	void CreateMaterialFromSelectedTextures();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;

	// bCustomMaterialName이 true일때만 MaterialName을 변경할 수 있다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bCustomMaterialName")) 
	FString MaterialName = TEXT("M_");

#pragma endregion

#pragma region SupportedTextureNames

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> BaseColorArray = {
		TEXT("_BaseColor"),
		TEXT("_Albedo"),
		TEXT("_Diffuse"),
		TEXT("_diff")
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> MetallicArray = {
		TEXT("_Metallic"),
		TEXT("_metal")
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> RoughnessArray = {
		TEXT("_Roughness"),
		TEXT("_RoughnessMap"),
		TEXT("_rough")
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> NormalArray = {
		TEXT("_Normal"),
		TEXT("_NormalMap"),
		TEXT("_nor")
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> AmbientOcclusionArray = {
		TEXT("_AmbientOcclusion"),
		TEXT("_AmbientOcclusionMap"),
		TEXT("_AO")
	};

#pragma endregion

private:
#pragma region QuickMaterialCreation

	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProccess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck); // 생성하려는 이름이 이미 있는지 확인
	TObjectPtr<UMaterial> CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial); // Material 생성

#pragma endregion

};
