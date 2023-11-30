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
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void SetHealth(float Percent);

    void SetTimerText(float RemainingTimeInSeconds);
    void SetAmmoInfoText(FText NewAmmoInfo);

public:
    UPROPERTY(EditAnywhere, Replicated, meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* TimeRemainingText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* AmmoInfoText;
};
