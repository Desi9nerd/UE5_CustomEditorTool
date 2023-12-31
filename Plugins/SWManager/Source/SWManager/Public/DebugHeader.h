﻿#pragma once
#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"

namespace DebugHeader
{
	void Print(const FString& Message, const FColor& Color)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
		}
	}

	void PrintLog(const FString& Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}

	EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true)
	{
		if (bShowMsgAsWarning)
		{
			// Warning과 잘못됬다는 문구(Message)를 띄우고 리턴한다
			FText MsgTitle = FText::FromString(TEXT("Warning"));

			return FMessageDialog::Open(MsgType, FText::FromString(Message), &MsgTitle);
		}
		else
		{	// 문구(Message)를 띄우고 리턴한다
			return FMessageDialog::Open(MsgType, FText::FromString(Message));
		}
	}

	void ShowNotifyInfo(const FString& Message)
	{
		FNotificationInfo NotifyInfo(FText::FromString(Message));
		NotifyInfo.bUseLargeFont = true;
		NotifyInfo.FadeOutDuration = 7.0f; // NotifyInfo 메시지를 7초 동안 띄운다

		FSlateNotificationManager::Get().AddNotification(NotifyInfo);
	}
}