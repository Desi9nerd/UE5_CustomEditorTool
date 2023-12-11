#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

/** Actor�� SRT(Scale, Rotate, Transform)�� �����ϴ� ����� Tool�� �����ϴ� Ŭ����
 *  UEditorUtilityWidget ����� ����
 */
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

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
