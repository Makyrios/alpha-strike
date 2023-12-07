// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_MenuWidget.h"
#include "UI/HUD/AS_MenuHUD.h"
#include "Components/EditableText.h"
#include "Components/VerticalBox.h"
#include "Components/Slider.h"
#include "AS_GameInstance.h"

void UAS_MenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (VolumeSlider)
    {
        VolumeSlider->OnValueChanged.AddDynamic(this, &UAS_MenuWidget::OnVolumeSliderValueChanged);

        AS_GameInstance = (!AS_GameInstance) ? GetGameInstance<UAS_GameInstance>() : AS_GameInstance;
        if (!AS_GameInstance) return;

        VolumeSlider->SetValue(AS_GameInstance->GetMasterSoundVolume());
    }
}

void UAS_MenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    Super::OnAnimationFinished_Implementation(Animation);

    if (Animation != JoinAnimation) return;

    if (!GetOwningPlayer()) return;

    auto AAS_HUD = GetOwningPlayer()->GetHUD<AAS_MenuHUD>();
    if (!AAS_HUD) return;

    AAS_HUD->HandleMenuAction();
}

void UAS_MenuWidget::OnVolumeSliderValueChanged(float Value)
{
    AS_GameInstance = (!AS_GameInstance) ? GetGameInstance<UAS_GameInstance>() : AS_GameInstance;
    if (!AS_GameInstance) return;

    AS_GameInstance->SetMasterSoundVolume(Value);
}

void UAS_MenuWidget::SetNameText(FText Name)
{
    if (NameBox)
    {
        NameBox->SetText(Name);
    }
}

FText UAS_MenuWidget::GetNameText()
{
    if (!NameBox) return FText();
    return NameBox->GetText();
}

void UAS_MenuWidget::PlayJoinAnimation()
{
    if (!JoinAnimation) return;
    PlayAnimation(JoinAnimation);
}
