#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/**	�Ʒ� 3������ ��ɵ��� �����ϴ� Tool�� ����� Ŭ����
 *	1. ����� �̸��� Actor���� �����ϴ� ���
 *	2. ������ Actor�� ������ Offset�������� ������ ����ŭ �����ϴ� ���
 *	3. Actor�� SRT(Scale, Rotate, Transform)�� �����ϴ� ���
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

USTRUCT(BlueprintType)
struct FRandomActorRotation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotYaw = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotYaw")) // bRandomizeRotYaw == true�� ���� ����
	float RotYawMin = -45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotYaw"))
	float RotYawMax = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotPitch = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotPitch"))
	float RotPitchMin = -45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotPitch"))
	float RotPitchMax = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRandomizeRotRoll = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotRoll"))
	float RotRollMin = -45.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotRoll"))
	float RotRollMax = 45.f;
};

UCLASS()
class SWMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
#pragma region ActorBatchSelection

	UFUNCTION(BlueprintCallable) // WBP�� Graph�� ������� ��ư Ŭ�� �� ����ǵ�����
	void SelectAllActorsWithSimilarName(); // ����� �̸��� ��� Actor�� ����

	// "ActorBatchSelection"�� WBP�� ActorBatchSelectionDetailsView�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;

#pragma endregion

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

//** Actor�� �������� Transform
#pragma region RandomizeActorTransform

	UFUNCTION(BlueprintCallable)
	void RandomizeActorTransform(); // Actor�� �������� Transform

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform")
	FRandomActorRotation RandomActorRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform")
	bool bRandomizeScale = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform", meta = (EditCondition = "bRandomizeScale"))
	float ScaleMin = 0.8f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform", meta = (EditCondition = "bRandomizeScale"))
	float ScaleMax = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform")
	bool bRandomizeOffset = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform", meta = (EditCondition = "bRandomizeOffset"))
	float OffsetMin = -50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomizeActorTransform", meta = (EditCondition = "bRandomizeOffset"))
	float OffsetMax = 50.f;

#pragma endregion

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
