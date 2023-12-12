#pragma once
#include "Framework/Commands/Commands.h"

/**
 * 
 */

class SWMANAGER_API FSWManagerUICommands
	: public TCommands<FSWManagerUICommands> // TCommands ���ø� Ŭ���� ��ӹ���
{
public:
	FSWManagerUICommands() : TCommands<FSWManagerUICommands>(
		TEXT("SWManager"),
		FText::FromString(TEXT("SW Manager UI Commands")),
		NAME_None,
		TEXT("SWManager")
	) {} // ������ Initialize

	virtual void RegisterCommands() override;
};