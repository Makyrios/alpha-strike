// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ALPHASTRIKE_API UAS_HUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    void SetHealth(float Percent);
    void SetShield(float Percent);

    void SetTimerText(float RemainingTimeInSeconds);
    void SetAmmoInfoText(FText NewAmmoInfo);

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UProgressBar* ShieldBar;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* TimeRemainingText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* AmmoInfoText;
};
