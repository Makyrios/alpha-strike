// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_StartGameWidget.h"
#include "Components/TextBlock.h"

void UAS_StartGameWidget::InitializeWidget(float DelayStartTimer)
{
    StartGameDelay = DelayStartTimer;
    Timer = StartGameDelay;
    bStartTimer = true;
}

void UAS_StartGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bStartTimer)
    {
        Timer -= InDeltaTime;
        if (StartGameDelay - Timer < 1)
        {
            DisplayText->SetText(FText::FromString("Get Ready!"));
        }
        else if (FMath::CeilToInt(Timer) == 0)
        {
            DisplayText->SetText(FText::FromString("Go!"));
        }
        else if (Timer > 0)
        {
            DisplayText->SetText(FText::AsNumber(FMath::CeilToInt(Timer)));
        }
        else
        {
            RemoveFromParent();
        }
    }
}
