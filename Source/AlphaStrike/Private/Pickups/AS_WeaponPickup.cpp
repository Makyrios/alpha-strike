// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Pickups/AS_WeaponPickup.h"
#include "Characters/AS_Character.h"
#include "Weapons/AS_BaseWeapon.h"
#include "Components/AS_CombatComponent.h"
#include "Components/AS_AmmoComponent.h"

AAS_WeaponPickup::AAS_WeaponPickup()
{
    bReplicates = true;
    StaticMesh->SetIsReplicated(true);
}

void AAS_WeaponPickup::PickupInteract(TArray<AActor*>& InteractedActors)
{
    Super::PickupInteract(InteractedActors);

    if (!WeaponClass || InteractedActors.Num() == 0) return;

    for (AActor* InteractedActor : InteractedActors)
    {
        if (InteractedActor->GetLocalRole() != ENetRole::ROLE_Authority) continue;

        if (AAS_Character* Character = Cast<AAS_Character>(InteractedActor))
        {
            UAS_CombatComponent* CombatComponent = Character->GetCombatComponent();
            if (!CombatComponent) continue;

            // If character already has this weapon
            // add ammo
            if (AAS_BaseWeapon* ExistingWeapon = CombatComponent->FindWeaponOfClass(WeaponClass))
            {
                AddAmmoInBag(ExistingWeapon);
                Despawn();

                break;
            }

            AAS_BaseWeapon* CreatedWeapon = SpawnWeapon(Character);
            if (!CreatedWeapon) continue;

            AttachWeaponToCharacter(Character, CreatedWeapon);
            Character->UpdateHUDInventoryInfo();
            Despawn();

            break;
        }
    }
}

AAS_BaseWeapon* AAS_WeaponPickup::SpawnWeapon(AAS_Character* Character)
{
    if (!Character->GetMesh()) return nullptr;

    FActorSpawnParameters WeaponSpawnParameters;
    WeaponSpawnParameters.Instigator = Character;
    WeaponSpawnParameters.Owner = Character;

    AAS_BaseWeapon* CreatedWeapon = GetWorld()->SpawnActor<AAS_BaseWeapon>(WeaponClass, WeaponSpawnParameters);

    if (CreatedWeapon && CreatedWeapon->GetWeaponMesh())
    {
        CreatedWeapon->GetWeaponMesh()->SetVisibility(false);
        return CreatedWeapon;
    }

    return nullptr;
}

void AAS_WeaponPickup::AttachWeaponToCharacter(AAS_Character* Character, AAS_BaseWeapon* Weapon)
{
    if (!Character || !Weapon) return;

    UAS_CombatComponent* CombatComponent = Character->GetCombatComponent();
    if (!CombatComponent) return;

    CombatComponent->AddWeaponToInventory(Weapon);

    const FAttachmentTransformRules AttachmentTransformRules(
        EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

    Weapon->AttachToComponent(Character->GetMesh(), AttachmentTransformRules, Weapon->GetMuzzleSocketName());
}

void AAS_WeaponPickup::AddAmmoInBag(AAS_BaseWeapon* Weapon)
{
    if (Weapon && Weapon->GetAmmoComponent())
    {
        Weapon->GetAmmoComponent()->AddAmmoInBag(AmmoRefillAmount);
    }
}
