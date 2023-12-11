#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/** Actor의 SRT(Scale, Rotate, Transform)을 조작하는 기능의 Tool을 구현하는 클래스
 *  UEditorUtilityWidget 상속을 받음
 */
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

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
