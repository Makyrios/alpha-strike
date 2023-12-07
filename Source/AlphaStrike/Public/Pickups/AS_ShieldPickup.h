// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Pickups/AS_Pickup.h"
#include "AS_ShieldPickup.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_ShieldPickup : public AAS_Pickup
{
    GENERATED_BODY()

protected:
    virtual void PickupInteract(TArray<AActor*>& InteractedActors);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Shield", meta = (AllowPrivateAccess = "true"))
    float ShieldAmount = 20.f;
};
