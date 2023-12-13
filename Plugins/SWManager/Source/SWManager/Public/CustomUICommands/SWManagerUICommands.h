#pragma once
#include "Framework/Commands/Commands.h"

/** 단축키 등록하기. Custom Editor Hot Key
 *  단축키를 등록한다. SWManger.cpp에서 이 클래스를 Register하여 사용한다.
 */

class SWMANAGER_API FSWManagerUICommands
	: public TCommands<FSWManagerUICommands> // TCommands 템플릿 클래스 상속받음
{
public:
	FSWManagerUICommands() : TCommands<FSWManagerUICommands>(
		TEXT("SWManager"),
		FText::FromString(TEXT("SW Manager UI Commands")),
		NAME_None,
		TEXT("SWManager")
	) {} // 생성자 Initialize

	virtual void RegisterCommands() override; // 단축키 등록하기

	TSharedPtr<FUICommandInfo> LockActorSelection; // Lock 단축키 등록에 사용할 command ID
	TSharedPtr<FUICommandInfo> UnlockActorSelection; // Unlock 단축키 등록에 사용할 command ID
};