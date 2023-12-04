// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Pickups/AS_Pickup.h"
#include "AS_WeaponPickup.generated.h"

class AAS_BaseWeapon;
class AAS_Character;
class UAS_CombatComponent;

UCLASS()
class ALPHASTRIKE_API AAS_WeaponPickup : public AAS_Pickup
{
    GENERATED_BODY()

public:
    AAS_WeaponPickup();

private:
    UPROPERTY(EditAnywhere, Category = "AS|Weapon")
    TSubclassOf<AAS_BaseWeapon> WeaponClass;

    UPROPERTY(EditAnywhere, Category = "AS|Weapon")
    int AmmoRefillAmount = 60;

protected:
    virtual void PickupInteract(TArray<AActor*>& InteractedActors) override;

private:
    AAS_BaseWeapon* SpawnWeapon(AAS_Character* Character);

    void AttachWeaponToCharacter(AAS_Character* Character, AAS_BaseWeapon* Weapon);

    void AddAmmoInBag(AAS_BaseWeapon* CombatComponent);

    UFUNCTION(Client, Reliable)
    void Client_UpdateHUDInventory(AAS_Character* Char, const TArray<AAS_BaseWeapon*>& WeaponArray, int CurrentWeaponIndex);
};
