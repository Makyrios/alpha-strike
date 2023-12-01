// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_Types.h"
#include "AS_CombatComponent.generated.h"

class AAS_BaseWeapon;
class AAS_Character;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALPHASTRIKE_API UAS_CombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "AS|Movement")
    float MaxAimWalkSpeed = 300.f;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Combat")
    float AimingFOV = 50.f;

public:
    UAS_CombatComponent();
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    void Aim();
    void StopAim();

    void Fire();
    void StopFire();

    void ScrollWeaponUp();
    void ScrollWeaponDown();

public:
    FORCEINLINE bool IsAiming() const { return bIsAiming; }
    FORCEINLINE AAS_BaseWeapon* GetEquippedWeapon() const { return EquippedWeapon; }

    EWeaponType GetEquippedWeaponType() const;
    FVector GetStartMuzzlePoint() const;
    FVector GetEndMuzzlePoint() const;
    TArray<AAS_BaseWeapon*> GetWeaponInventory() const { return WeaponInventory; }
    void AddWeaponToInventory(AAS_BaseWeapon* NewWeapon);

protected:
    virtual void BeginPlay() override;

    void SpawnWeapon();

private:
    UPROPERTY()
    AAS_Character* PlayerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Combat")
    TSubclassOf<AAS_BaseWeapon> EquippedWeaponClass;

    UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
    AAS_BaseWeapon* EquippedWeapon = nullptr;
    TArray<AAS_BaseWeapon*> WeaponInventory;
    int EquippedWeaponIndex;
    UPROPERTY(Replicated)
    bool bIsAiming = false;

private:
    UFUNCTION()
    void OnRep_EquippedWeapon();

    UFUNCTION(Server, Reliable)
    void Server_SetAim(bool bAim);
    void ChangeWeapon(int WeaponIndex);
    void UpdateHUDAmmoInfo();
};
