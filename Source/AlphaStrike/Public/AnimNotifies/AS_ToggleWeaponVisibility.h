// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AS_ToggleWeaponVisibility.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_ToggleWeaponVisibility : public UAnimNotify
{
    GENERATED_BODY()

private:
    virtual void Notify(
        USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
