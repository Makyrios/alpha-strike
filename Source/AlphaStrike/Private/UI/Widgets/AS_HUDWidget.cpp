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

void UAS_HUDWidget::UpdateInventoryInfo()
{
    if (!PistolIcon || !RifleIcon || !SniperRifleIcon) return;

    AAS_Character* OwningCharacter = GetOwningPlayer() ? GetOwningPlayer()->GetPawn<AAS_Character>() : nullptr;
    if (!OwningCharacter || !OwningCharacter->GetCombatComponent()) return;

    const TArray<AAS_BaseWeapon*> WeaponArray = OwningCharacter->GetCombatComponent()->GetWeaponInventory();
    int CurrentWeaponIndex = OwningCharacter->GetCombatComponent()->GetEquippedWeaponIndex();

    UpdateWeaponIcons(WeaponArray, CurrentWeaponIndex);
}

void UAS_HUDWidget::UpdateWeaponIcons(const TArray<AAS_BaseWeapon*>& WeaponArray, int CurrentWeaponIndex)
{
    for (int i = 0; i < WeaponArray.Num(); i++)
    {
        if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_Pistol)
        {
            PistolIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                PistolIcon->SetOpacity(1);
            }
            else
            {
                PistolIcon->SetOpacity(0.5);
            }
        }
        else if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_Rifle)
        {
            RifleIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                RifleIcon->SetOpacity(1);
            }
            else
            {
                RifleIcon->SetOpacity(0.5);
            }
        }
        else if (WeaponArray[i] && WeaponArray[i]->GetWeaponType() == EWeaponType::EWT_SniperRifle)
        {
            SniperRifleIcon->SetVisibility(ESlateVisibility::Visible);
            if (CurrentWeaponIndex == i)
            {
                SniperRifleIcon->SetOpacity(1);
            }
            else
            {
                SniperRifleIcon->SetOpacity(0.5);
            }
        }
    }
}
