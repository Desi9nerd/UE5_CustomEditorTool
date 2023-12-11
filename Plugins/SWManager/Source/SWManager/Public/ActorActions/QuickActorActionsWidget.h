#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/** Actor�� SRT(Scale, Rotate, Transform)�� �����ϴ� ����� Tool�� �����ϴ� Ŭ����
 *  UEditorUtilityWidget ����� ����
 */
UENUM(BlueprintType)
enum class E_DuplicationAxis : uint8
{
	EDA_XAxis UMETA(DisplayName = "X Axis"),
	EDA_YAxis UMETA(DisplayName = "Y Axis"),
	EDA_ZAxis UMETA(DisplayName = "Z Axis"),
	EDA_MAX UMETA(DisplayName = "Default Max")
};

UCLASS()
class SWMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) // WBP�� Graph�� ������� ��ư Ŭ�� �� ����ǵ�����
	void SelectAllActorsWithSimilarName(); // ����� �̸��� ��� Actor�� ����

	// "ActorBatchSelection"�� WBP�� ActorBatchSelectionDetailsView�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;

//** Actor ����. WBP�� ActorBatchDuplicationDetailsView/Button�� �Ʒ����� ����
#pragma region ActorBatchDuplication 

	UFUNCTION(BlueprintCallable)
	void DuplicateActors(); // Actor ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::EDA_XAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	int32 NumberOfDuplicates = 5; // ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	float OffsetDist = 300.f; // ���� �� Actor �� ����

#pragma endregion

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
