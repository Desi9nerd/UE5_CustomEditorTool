#include "ActorActions/QuickActorActionsWidget.h"
#include "Subsystems/EditorActorSubsystem.h"

bool UQuickActorActionsWidget::GetEditorActorSubsystem()
{
	if (false == IsValid(EditorActorSubsystem))
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>(); // 할당
	}

	return IsValid(EditorActorSubsystem); // EditorActorSubsystem 존재한다면 true리턴
}
