#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

/** Material ���� EditorUtilityWidget
 *  ������ Texture�� ����ؼ� Material ����
 *	bCustomMaterailName==true�� �̸��� ���� �� �ְ�, false�� Texture�̸��� M_ ���ξ ���� �̸����� ���� 
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

	// bCustomMaterialName�� true�϶��� MaterialName�� ������ �� �ִ�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bCustomMaterialName")) 
	FString MaterialName = TEXT("M_");

#pragma endregion

private:
#pragma region QuickMaterialCreation

	bool ProcessSelectedData(const TArray<FAssetData>& SelectedDataToProccess, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePackagePath);
	bool CheckIsNameUsed(const FString& FolderPathToCheck, const FString& MaterialNameToCheck); // �����Ϸ��� �̸��� �̹� �ִ��� Ȯ��
	TObjectPtr<UMaterial> CreateMaterialAsset(const FString& NameOfTheMaterial, const FString& PathToPutMaterial); // Material ����

#pragma endregion

};
