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

void UQuickActorActionsWidget::DuplicateActors() // Actor 복제
{
	if (false == GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors(); // 선택된 Actor들을 담는 변수
	uint32 Counter = 0; // 복제된 Actor 수 기록할 변수

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("선택된 Actor가 없습니다."));
		return;
	}

	if (NumberOfDuplicates <= 0 || OffsetDist == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("'복제를 희망하는 수'와 '복제 간격'을 올바르게 기입해주세요."));
		return;
	}

	for (AActor* SelectedActor : SelectedActors) // 선택된 Actor들을 for문 돌아 복제
	{
		if (false == IsValid(SelectedActor)) continue; // 선택된 Actor가 없으면 continue

		for (int32 i = 0; i < NumberOfDuplicates; i++) // NumberOfDuplicates 수 만큼 복제
		{
			//** Actor 복제
			AActor* DuplicatedActor = EditorActorSubsystem->DuplicateActor(SelectedActor, SelectedActor->GetWorld()); 

			if (false == IsValid(DuplicatedActor)) continue;

			//** 복제 후 Offset 이동
			const float DuplicationOffsetDist = (i + 1) * OffsetDist;

			switch (AxisForDuplication)
			{
			case E_DuplicationAxis::EDA_XAxis:

				DuplicatedActor->AddActorWorldOffset(FVector(DuplicationOffsetDist, 0.f, 0.f));
				break;

			case E_DuplicationAxis::EDA_YAxis:

				DuplicatedActor->AddActorWorldOffset(FVector(0.f, DuplicationOffsetDist, 0.f));
				break;

			case E_DuplicationAxis::EDA_ZAxis:

				DuplicatedActor->AddActorWorldOffset(FVector(0.f, 0.f, DuplicationOffsetDist));
				break;

			case E_DuplicationAxis::EDA_MAX:
				break;

			default:
				break;
			}

			//** 복제된 Actor들 선택
			EditorActorSubsystem->SetActorSelectionState(DuplicatedActor, true);
			Counter++; // 복제된 Actor 수 기록
		}
	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("총 ") +
			FString::FromInt(Counter) + TEXT(" 개의 Actor가 성공적으로 복제되었습니다."));
	}
}

void UQuickActorActionsWidget::RandomizeActorTransform() // Actor를 랜덤으로 Transform
{
	const bool ConditionNotSet =
		false == RandomActorRotation.bRandomizeRotYaw &&
		false == RandomActorRotation.bRandomizeRotPitch &&
		false == RandomActorRotation.bRandomizeRotRoll;

	if (ConditionNotSet)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No variation condition specified"));
		return;
	}

	if (false == GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors(); // 선택된 Actor들을 담음
	uint32 Counter = 0; // Transform된 Actor 수를 기록할 변수

	if (SelectedActors.Num() == 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("선택된 Actor가 없습니다."));
		return;
	}

	for (AActor* SelectedActor : SelectedActors)
	{
		if (false == IsValid(SelectedActor)) continue; 

		if (RandomActorRotation.bRandomizeRotYaw) // Yaw
		{
			const float RandomRotYawValue = FMath::RandRange(RandomActorRotation.RotYawMin, RandomActorRotation.RotYawMax);

			SelectedActor->AddActorWorldRotation(FRotator(0.f, RandomRotYawValue, 0.f));
		}
		if (RandomActorRotation.bRandomizeRotPitch) // Pitch
		{
			const float RandomRotPitchValue = FMath::RandRange(RandomActorRotation.RotPitchMin, RandomActorRotation.RotPitchMax);

			SelectedActor->AddActorWorldRotation(FRotator(RandomRotPitchValue, 0.f, 0.f));
		}
		if (RandomActorRotation.bRandomizeRotRoll) // Roll
		{
			const float RandomRotRollValue = FMath::RandRange(RandomActorRotation.RotRollMin, RandomActorRotation.RotRollMax);

			SelectedActor->AddActorWorldRotation(FRotator(0.f, 0.f, RandomRotRollValue));
		}

		const bool bShouldIncreaseCounter =
			RandomActorRotation.bRandomizeRotYaw ||
			RandomActorRotation.bRandomizeRotPitch ||
			RandomActorRotation.bRandomizeRotRoll;

		if (bShouldIncreaseCounter) Counter++; // Transform된 Actor 수++

	}

	if (Counter > 0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("총 ") +
			FString::FromInt(Counter) + TEXT(" 개의 Actor들이 Tranform 되었습니다."));
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
