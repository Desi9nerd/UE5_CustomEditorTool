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

private:
	UPROPERTY()
	TObjectPtr<class UEditorActorSubsystem> EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
