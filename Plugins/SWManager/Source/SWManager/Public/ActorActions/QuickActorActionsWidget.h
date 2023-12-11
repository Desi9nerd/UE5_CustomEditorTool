#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/**	아래 3가지의 기능들을 수행하는 Tool를 만드는 클래스
 *	1. 비슷한 이름의 Actor들을 선택하는 기능
 *	2. 선택한 Actor를 설정한 Offset간격으로 지정한 수만큼 복제하는 기능
 *	3. Actor의 SRT(Scale, Rotate, Transform)을 조작하는 기능
 *  UEditorUtilityWidget 상속을 받음
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotYaw")) // bRandomizeRotYaw == true일 때만 적용
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

	UFUNCTION(BlueprintCallable) // WBP의 Graph에 노출시켜 버튼 클릭 시 실행되도록함
	void SelectAllActorsWithSimilarName(); // 비슷한 이름의 모든 Actor를 선택

	// "ActorBatchSelection"를 WBP의 ActorBatchSelectionDetailsView에 연결
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;

#pragma endregion

//** Actor 복제. WBP의 ActorBatchDuplicationDetailsView/Button에 아래변수 연결
#pragma region ActorBatchDuplication 

	UFUNCTION(BlueprintCallable)
	void DuplicateActors(); // Actor 복제

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::EDA_XAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	int32 NumberOfDuplicates = 5; // 복제 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchDuplication")
	float OffsetDist = 300.f; // 복제 시 Actor 간 간격

#pragma endregion

//** Actor를 랜덤으로 Transform
#pragma region RandomizeActorTransform

	UFUNCTION(BlueprintCallable)
	void RandomizeActorTransform(); // Actor를 랜덤으로 Transform

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
