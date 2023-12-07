// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_PauseWidget.generated.h"

class UButton;
class AAS_HUD;
class USlider;
class UAS_GameInstance;

UCLASS()
class ALPHASTRIKE_API UAS_PauseWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ExitButton;

    UPROPERTY(meta = (BindWidget))
    USlider* VolumeSlider;

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnPlayButtonClicked();

    UFUNCTION()
    void OnExitButtonClicked();

    UFUNCTION()
    void OnVolumeSliderValueChanged(float Value);

    UPROPERTY()
    UAS_GameInstance* AS_GameInstance;

    UPROPERTY()
    AAS_HUD* AS_HUD;
};
