// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Weapons/AS_BaseWeapon.h"
#include "Characters/AS_Character.h"
#include "Components/AS_CombatComponent.h"

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

void UAS_HUDWidget::SetScoreGoalText(int32 ScoreGoal)
{
    if (ScoreGoalText)
    {
        const FString ScoreGoalString = FString::FromInt(ScoreGoal);
        ScoreGoalText->SetText(FText::FromString(ScoreGoalString));
    }
}

void UAS_HUDWidget::UpdateInventoryInfo()
{
    if (!PistolIcon || !RifleIcon || !SniperRifleIcon) return;

    const APlayerController* OwningController = GetOwningPlayer();
    if (!OwningController) return;

    const AAS_Character* OwningCharacter = OwningController->GetPawn<AAS_Character>();
    if (!OwningCharacter || !OwningCharacter->GetCombatComponent()) return;

    const TArray<AAS_BaseWeapon*> WeaponArray = OwningCharacter->GetCombatComponent()->GetWeaponInventory();
    const int CurrentWeaponIndex = OwningCharacter->GetCombatComponent()->GetEquippedWeaponIndex();

    UpdateWeaponIcons(WeaponArray, CurrentWeaponIndex);
}

void UAS_HUDWidget::UpdateWeaponIcons(const TArray<AAS_BaseWeapon*>& WeaponArray, int CurrentWeaponIndex)
{
    if (!PistolIcon || !RifleIcon || !SniperRifleIcon || !PistolLine || !RifleLine || !SniperRifleLine) return;

    PistolIcon->SetVisibility(ESlateVisibility::Hidden);
    RifleIcon->SetVisibility(ESlateVisibility::Hidden);
    SniperRifleIcon->SetVisibility(ESlateVisibility::Hidden);

    PistolLine->SetVisibility(ESlateVisibility::Hidden);
    RifleLine->SetVisibility(ESlateVisibility::Hidden);
    SniperRifleLine->SetVisibility(ESlateVisibility::Hidden);

    for (int i = 0; i < WeaponArray.Num(); i++)
    {
        if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_Pistol)
        {
            PistolIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                PistolIcon->SetOpacity(1);
                PistolLine->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                PistolIcon->SetOpacity(0.5);
                PistolLine->SetVisibility(ESlateVisibility::Hidden);
            }
        }
        else if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_Rifle)
        {
            RifleIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                RifleIcon->SetOpacity(1);
                RifleLine->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                RifleIcon->SetOpacity(0.5);
                RifleLine->SetVisibility(ESlateVisibility::Hidden);
            }
        }
        else if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_SniperRifle)
        {
            SniperRifleIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                SniperRifleIcon->SetOpacity(1);
                SniperRifleLine->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                SniperRifleIcon->SetOpacity(0.5);
                SniperRifleLine->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }
}
