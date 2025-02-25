// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_DamageWidget.generated.h"

class UWidgetAnimation;

UCLASS()
class ALPHASTRIKE_API UAS_DamageWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void PlayDamageAnimation();

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* DamageAnimation;
};
