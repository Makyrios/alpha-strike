// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_MenuWidget.generated.h"

class UButton;
class UEditableText;
class UVerticalBox;
class USlider;
class UAS_GameInstance;

UCLASS()
class ALPHASTRIKE_API UAS_MenuWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetNameText(FText Name);
    FText GetNameText();

protected:
    void NativeConstruct() override;

public:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PrimaryOptions;

    UPROPERTY(meta = (BindWidget))
    UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    UButton* JoinButton;

    UPROPERTY(meta = (BindWidget))
    UButton* ApplyNameButton;

    UPROPERTY(meta = (BindWidget))
    UEditableText* NameBox;

    UPROPERTY(meta = (BindWidget))
    UEditableText* IP_AddressBox;

    UPROPERTY(meta = (BindWidget))
    UButton* SingleDeathmatchButton;

    UPROPERTY(meta = (BindWidget))
    UButton* TeamDeathmatchButton;

    UPROPERTY(meta = (BindWidget))
    USlider* VolumeSlider;

private:
    UFUNCTION()
    void OnVolumeSliderValueChanged(float Value);

    UPROPERTY()
    UAS_GameInstance* AS_GameInstance;
};
