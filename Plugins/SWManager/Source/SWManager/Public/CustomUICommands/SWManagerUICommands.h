#pragma once
#include "Framework/Commands/Commands.h"

/**
 * 
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

	virtual void RegisterCommands() override;
};