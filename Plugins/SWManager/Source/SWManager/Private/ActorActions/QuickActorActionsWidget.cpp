#include "ActorActions/QuickActorActionsWidget.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "DebugHeader.h"

void UQuickActorActionsWidget::SelectAllActorsWithSimilarName()
{
	if (false == GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	uint32 SelectionCounter = 0; // 비슷한 이름으로 선택된 Actor의 수

	//** 예외처리
	if (SelectedActors.Num() == 0) // Actor를 선택하지 않은 경우
	{
		DebugHeader::ShowNotifyInfo(TEXT("선택된 Actor가 없습니다."));
		return;
	}
	if (SelectedActors.Num() > 1)  // Actor를 2개 이상 선택한 경우
	{
		DebugHeader::ShowNotifyInfo(TEXT("한가지 Actor만 선택할 수 있습니다. 다시 선택하십시오."));
		return;
	}

	//** 제대로 선택한 경우. Actor를 1개 선택한 경우 
	FString SelectedActorName = SelectedActors[0]->GetActorLabel(); // 선택한 Actor가 WorldOutlinear에 노출되는 이름
	const FString NameToSearch = SelectedActorName.LeftChop(4); // SelectedActorName에서 끝의 4글자를 제거하여 NameToSearch변수에 담음

	TArray<AActor*> AllLeveActors = EditorActorSubsystem->GetAllLevelActors(); // Level에 있는 모든 Actor

	for (AActor* ActorInLevel : AllLeveActors)
	{
		if (false == IsValid(ActorInLevel)) continue;

		// Actor의 이름에 NameToSearch가 포함되어 있다면
		if (ActorInLevel->GetActorLabel().Contains(NameToSearch, SearchCase))
		{
			// 해당 Actor를 선택하고 SelectionCounter를 증가시킨다
			EditorActorSubsystem->SetActorSelectionState(ActorInLevel, true);
			SelectionCounter++; // 비슷한 이름으로 선택된 Actor의 수++
		}
	}

	if (SelectionCounter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("총 ") +
			FString::FromInt(SelectionCounter) + TEXT(" 개의 Actor들이 성공적으로 선택되었습니다."));
	}
	else
	{
		DebugHeader::ShowNotifyInfo(TEXT("비슷한 이름의 Actor를 찾지 못했습니다."));
	}
}

bool UQuickActorActionsWidget::GetEditorActorSubsystem()
{
	if (false == IsValid(EditorActorSubsystem))
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>(); // 할당
	}

	return IsValid(EditorActorSubsystem); // EditorActorSubsystem 존재한다면 true리턴
}
