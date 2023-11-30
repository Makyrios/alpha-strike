// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"

void UAS_HUDWidget::SetHealth(float Percent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(Percent);
    }
}

void UAS_HUDWidget::SetShield(float Percent)
{
    if (ShieldBar)
    {
        ShieldBar->SetPercent(Percent);
    }
}

void UAS_HUDWidget::SetTimerText(float RemainingTimeInSeconds)
{
    if (TimeRemainingText && RemainingTimeInSeconds >= 0)
    {
        FString TimerString = UKismetStringLibrary::TimeSecondsToString(RemainingTimeInSeconds);
        TimerString.RemoveAt(5, 3);
        TimeRemainingText->SetText(FText::FromString(TimerString));
    }
}

void UAS_HUDWidget::SetAmmoInfoText(FText NewAmmoInfo)
{
    if (AmmoInfoText)
    {
        AmmoInfoText->SetText(NewAmmoInfo);
    }
}
