// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AnimNotifies/AS_ToggleWeaponVisibility.h"
#include "Characters/AS_Character.h"
#include "Weapons/AS_BaseWeapon.h"

void UAS_ToggleWeaponVisibility::Notify(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    if (!MeshComp) return;

    if (AAS_Character* Character = MeshComp->GetOwner<AAS_Character>())
    {
        if (Character->GetLocalRole() != ENetRole::ROLE_Authority) return;

        if (AAS_BaseWeapon* Weapon = Character->GetEquippedWeapon())
        {
            if (Weapon)
            {
                Weapon->Server_ToggleVisibility();
            }
        }
    }
}
