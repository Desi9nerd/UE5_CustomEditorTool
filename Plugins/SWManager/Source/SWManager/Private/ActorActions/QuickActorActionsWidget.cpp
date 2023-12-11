#include "ActorActions/QuickActorActionsWidget.h"
#include "Subsystems/EditorActorSubsystem.h"

bool UQuickActorActionsWidget::GetEditorActorSubsystem()
{
	if (false == IsValid(EditorActorSubsystem))
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>(); // �Ҵ�
	}

	return IsValid(EditorActorSubsystem); // EditorActorSubsystem �����Ѵٸ� true����
}
