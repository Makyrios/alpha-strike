// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "AS_HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
class AAS_BaseWeapon;

UCLASS()
class ALPHASTRIKE_API UAS_HUDWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetHealth(float Percent);
    void SetShield(float Percent);

    void SetTimerText(float RemainingTimeInSeconds);
    void SetAmmoInfoText(FText NewAmmoInfo);
    void SetScoreGoalText(int32 ScoreGoal);

    void UpdateInventoryInfo();

    void UpdateWeaponIcons(const TArray<AAS_BaseWeapon*>& WeaponArray, int CurrentWeaponIndex);

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UProgressBar* ShieldBar;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* TimeRemainingText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* AmmoInfoText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* ScoreGoalText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* PistolIcon;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* RifleIcon;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* SniperRifleIcon;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* PistolLine;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* RifleLine;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* SniperRifleLine;
};
