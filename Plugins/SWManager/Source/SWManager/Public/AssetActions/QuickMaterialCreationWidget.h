#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

/** Material ���� EditorUtilityWidget
 *  ������ Texture�� ����ؼ� Material ����
 *	bCustomMaterailName==true�� �̸��� ���� �� �ְ�, false�� Texture�̸��� M_ ���ξ ���� �̸����� ���� 
 */

UENUM(BlueprintType)
enum class E_ChannelPackingType : uint8
{
	ECPT_NoChannelPacking UMETA(DisplayName = "No Channel Packing"),
	ECPT_ORM UMETA(DisplayName = "OcclusionRoughnessMetallic"),
	ECPT_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class SWMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:

#pragma region QuickMaterialCreationCore

	UFUNCTION(BlueprintCallable) // WBP_QuickMaterialCreation�� �����
	void CreateMaterialFromSelectedTextures(); // ���õ� Texture�� Material ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	E_ChannelPackingType ChannelPackingType = E_ChannelPackingType::ECPT_NoChannelPacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCustomMaterialName = true;

	// bCustomMaterialName�� true�϶��� MaterialName�� ������ �� �ִ�.
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Supported Texture Names")
	TArray<FString> ORMArray = {
		TEXT("_arm"),
		TEXT("_OcclusionRoughnessMetallic"),
		TEXT("_ORM")
	};

#pragma endregion

private:
#pragma region QuickMaterialCreation

	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProccess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck); // �����Ϸ��� �̸��� �̹� �ִ��� Ȯ��
	TObjectPtr<UMaterial> CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial); // Material ����
	void Default_CreateMaterialNodes(TObjectPtr<UMaterial> CreatedMaterial, TObjectPtr<UTexture2D> SelectedTexture, uint32& PinsConnectedCounter); // Material Node ����
	void ORM_CreateMaterialNodes(TObjectPtr<UMaterial> CreatedMaterial, TObjectPtr<UTexture2D> SelectedTexture, uint32& PinsConnectedCounter); // Material Node ���� (Channel Packed Texture�� ���)

#pragma endregion

#pragma region CreateMaterialNodesConnectPins

	bool TryConnectBaseColor(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial); // BaseColor Node ������ �Ǿ� ������ false, �� �Ǿ� ������ �����ϰ� true ����
	bool TryConnectMetalic(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial);
	bool TryConnectRoughness(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial);
	bool TryConnectNormal(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial);
	bool TryConnectAO(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial);
	bool TryConnectORM(TObjectPtr<UMaterialExpressionTextureSample> TextureSampleNode, TObjectPtr<UTexture2D> SelectedTexture, TObjectPtr<UMaterial> CreatedMaterial); // Channel Packed Texture�� ���, AO, Roughness, Metallic�� ���ʴ�� �������� �� true ����

#pragma endregion

};
