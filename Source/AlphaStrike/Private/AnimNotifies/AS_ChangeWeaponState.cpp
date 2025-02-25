// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AnimNotifies/AS_ChangeWeaponState.h"
#include "Characters/AS_Character.h"
#include "Weapons/AS_BaseWeapon.h"

void UAS_ChangeWeaponState::NotifyBegin(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    if (!MeshComp) return;

    if (AAS_Character* Character = MeshComp->GetOwner<AAS_Character>())
    {
        if (AAS_BaseWeapon* Weapon = Character->GetEquippedWeapon())
        {
            Weapon->SetCanFire(false);
        }
    }
}

void UAS_ChangeWeaponState::NotifyEnd(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);

    if (!MeshComp) return;

    if (AAS_Character* Character = MeshComp->GetOwner<AAS_Character>())
    {
        if (AAS_BaseWeapon* Weapon = Character->GetEquippedWeapon())
        {
            Weapon->SetCanFire(true);
            Weapon->FinishChangeWeapon();
        }
    }
}
