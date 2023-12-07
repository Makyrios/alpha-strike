// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Pickups/AS_Pickup.h"
#include "AS_SpeedPickup.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_SpeedPickup : public AAS_Pickup
{
    GENERATED_BODY()

protected:
    virtual void PickupInteract(TArray<AActor*>& InteractedActors);

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Speed", meta = (AllowPrivateAccess = "true"))
    float SpeedBuffMultiplier = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Speed", meta = (AllowPrivateAccess = "true"))
    float SpeedBuffTime = 5.f;
};
