#pragma once

#include "Misc/MessageDialog.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Engine/Engine.h"

namespace DebugHeader
{
    // FIX: Changed FColor::MakeRandom() to FColor::MakeRandomColor()
    static void Print(const FString& Message, const FColor& Color = FColor::MakeRandomColor())
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.f, Color, Message);
        }
    }

    static void PrintLog(const FString& Message)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }

    static EAppReturnType::Type ShowMsgDialog(EAppMsgType::Type MsgType, const FString& Message, bool bShowMsgAsWarning = true)
    {
        if (bShowMsgAsWarning)
        {
            // Pass title by-value to prevent UE 5.7 deprecation error
            return FMessageDialog::Open(MsgType, FText::FromString(Message), FText::FromString(TEXT("Warning")));
        }
        else
        {
            return FMessageDialog::Open(MsgType, FText::FromString(Message));
        }
    }

    static void ShowNotifyInfo(const FString& Message)
    {
        FNotificationInfo NotifyInfo(FText::FromString(Message));
        NotifyInfo.bUseLargeFont = true;
        NotifyInfo.FadeOutDuration = 7.f;

        FSlateNotificationManager::Get().AddNotification(NotifyInfo);
    }
}