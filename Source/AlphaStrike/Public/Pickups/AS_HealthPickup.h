// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Pickups/AS_Pickup.h"
#include "AS_HealthPickup.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_HealthPickup : public AAS_Pickup
{
    GENERATED_BODY()

protected:
    virtual void PickupInteract(TArray<AActor*>& InteractedActors) override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Health", meta = (AllowPrivateAccess = "true"))
    float HealAmount = 20.f;
};
