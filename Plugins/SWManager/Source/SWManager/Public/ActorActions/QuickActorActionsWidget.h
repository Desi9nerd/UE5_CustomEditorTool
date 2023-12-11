#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/** Actor의 SRT(Scale, Rotate, Transform)을 조작하는 기능의 Tool을 구현하는 클래스
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

UCLASS()
class SWMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) // WBP의 Graph에 노출시켜 버튼 클릭 시 실행되도록함
	void SelectAllActorsWithSimilarName(); // 비슷한 이름의 모든 Actor를 선택

	// "ActorBatchSelection"를 WBP의 ActorBatchSelectionDetailsView에 연결
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;

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

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
