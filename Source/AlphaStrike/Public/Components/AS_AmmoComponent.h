// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_Types.h"
#include "AS_AmmoComponent.generated.h"

class AAS_BaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALPHASTRIKE_API UAS_AmmoComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAS_AmmoComponent();
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable)
    void Server_HandleWeaponFired();

    UFUNCTION(Server, Reliable)
    void Server_Reload();

    bool CanShoot() const;
    bool CanReload() const;

    void AddAmmoInBag(int AmmoAmount);

public:
    FORCEINLINE const FAmmoInfo& GetAmmoInfo() const { return AmmoInfo; }
    FText GetAmmoInfoAsText();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(ReplicatedUsing = OnRep_AmmoInfo, EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    FAmmoInfo AmmoInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    float ReloadTime = 2.0f;

private:
    UPROPERTY()
    AAS_BaseWeapon* OwnerWeapon;

    FTimerHandle ReloadTimer;

private:
    UFUNCTION()
    void OnRep_AmmoInfo();

    void ReloadAmmo();
};
