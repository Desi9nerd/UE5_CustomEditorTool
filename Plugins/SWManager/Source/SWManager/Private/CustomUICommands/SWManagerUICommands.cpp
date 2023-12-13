#include "CustomUICommands/SWManagerUICommands.h"

#define LOCTEXT_NAMESPACE "FSWManagerModule"

void FSWManagerUICommands::RegisterCommands() // 단축키 등록하기
{
	UI_COMMAND(
		LockActorSelection,
		"선택한 액터 LOCK",
		"Level에 있는 Actor가 선택되는 것을 방지한다.",
		EUserInterfaceActionType::Button, // 버튼을 누르면 trigger
		FInputChord(EKeys::W, EModifierKey::Alt) // 'W + Alt'버튼
	);

	UI_COMMAND(
		UnlockActorSelection,
		"선택한 액터 UNLOCK",
		"Lock된 Actor들 잠금 해제",
		EUserInterfaceActionType::Button, // 버튼을 누르면 trigger
		FInputChord(EKeys::W, EModifierKey::Alt | EModifierKey::Shift) // 'W + Alt/Shift'버튼
	);
}

#undef LOCTEXT_NAMESPACE