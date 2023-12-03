// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Weapons/AS_BaseWeapon.h"
#include "AS_RifleWeapon.generated.h"

/**
 *
 */
UCLASS()
class ALPHASTRIKE_API AAS_RifleWeapon : public AAS_BaseWeapon
{
    GENERATED_BODY()

private:
    // This delay cannot be less than FireDelay from base class
    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    float AutomaticFireDelay = 0.2f;

protected:
    virtual void Fire() override;
    virtual void StopFire() override;


private:
    FTimerHandle AutoFireHandle;

    void Firing();
};
