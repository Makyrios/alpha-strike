// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_PauseWidget.h"
#include "UI/HUD/AS_HUD.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "AS_GameInstance.h"

void UAS_PauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!PlayButton || !ExitButton || !VolumeSlider) return;

    PlayButton->OnClicked.AddDynamic(this, &UAS_PauseWidget::OnPlayButtonClicked);
    ExitButton->OnClicked.AddDynamic(this, &UAS_PauseWidget::OnExitButtonClicked);

    VolumeSlider->OnValueChanged.AddDynamic(this, &UAS_PauseWidget::OnVolumeSliderValueChanged);

    AS_GameInstance = (!AS_GameInstance) ? GetGameInstance<UAS_GameInstance>() : AS_GameInstance;
    if (!AS_GameInstance) return;

    VolumeSlider->SetValue(AS_GameInstance->GetMasterSoundVolume());
}

void UAS_PauseWidget::OnPlayButtonClicked()
{
    if (!GetOwningPlayer() || !GetOwningPlayer()->GetHUD()) return;

    AS_HUD = (!AS_HUD) ? Cast<AAS_HUD>(GetOwningPlayer()->GetHUD()) : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->Pause(false);
}

void UAS_PauseWidget::OnExitButtonClicked()
{
    if (!GetOwningPlayer() || !GetOwningPlayer()->GetHUD()) return;

    AS_HUD = (!AS_HUD) ? Cast<AAS_HUD>(GetOwningPlayer()->GetHUD()) : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ExitToMenu();
}

void UAS_PauseWidget::OnVolumeSliderValueChanged(float Value) 
{
    AS_GameInstance = (!AS_GameInstance) ? GetGameInstance<UAS_GameInstance>() : AS_GameInstance;
    if (!AS_GameInstance) return;

    AS_GameInstance->SetMasterSoundVolume(Value);
}
