// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_DamageWidget.h"

void UAS_DamageWidget::PlayDamageAnimation() 
{
    if (!DamageAnimation) return;

    if (!IsAnimationPlaying(DamageAnimation))
    {
        PlayAnimation(DamageAnimation);
    }
}
