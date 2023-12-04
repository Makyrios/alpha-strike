// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_AmmoComponent.generated.h"

class AAS_BaseWeapon;

USTRUCT(BlueprintType)
struct FAmmoInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bFiniteAmmo;

    UPROPERTY()
    int32 CurrentAmmo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bFiniteAmmo"))
    int32 MaxAmmoInClip;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bFiniteAmmo"))
    int32 AmmoInBag;
};

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

    FText GetAmmoInfoAsText();

    FORCEINLINE const FAmmoInfo& GetAmmoInfo() const { return AmmoInfo; }

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(ReplicatedUsing = OnRep_AmmoInfo, EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    FAmmoInfo AmmoInfo;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    float ReloadTime;

private:
    FTimerHandle ReloadTimer;

    UPROPERTY()
    AAS_BaseWeapon* OwnerWeapon;

private:
    void ReloadAmmo();

    UFUNCTION()
    void OnRep_AmmoInfo();
};
