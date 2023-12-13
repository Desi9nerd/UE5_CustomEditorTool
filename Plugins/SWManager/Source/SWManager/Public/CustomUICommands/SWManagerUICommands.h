#pragma once
#include "Framework/Commands/Commands.h"

/** ����Ű ����ϱ�. Custom Editor Hot Key
 *  ����Ű�� ����Ѵ�. SWManger.cpp���� �� Ŭ������ Register�Ͽ� ����Ѵ�.
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

	virtual void RegisterCommands() override; // ����Ű ����ϱ�

	TSharedPtr<FUICommandInfo> LockActorSelection; // Lock ����Ű ��Ͽ� ����� command ID
	TSharedPtr<FUICommandInfo> UnlockActorSelection; // Unlock ����Ű ��Ͽ� ����� command ID
};