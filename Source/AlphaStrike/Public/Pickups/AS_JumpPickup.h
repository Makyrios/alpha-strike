// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Pickups/AS_Pickup.h"
#include "AS_JumpPickup.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_JumpPickup : public AAS_Pickup
{
    GENERATED_BODY()

protected:
    virtual void PickupInteract(TArray<AActor*>& InteractedActors);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Jump", meta = (AllowPrivateAccess = "true"))
    float JumpBuffMultiplier = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Jump", meta = (AllowPrivateAccess = "true"))
    float JumpBuffTime = 5.f;
};
