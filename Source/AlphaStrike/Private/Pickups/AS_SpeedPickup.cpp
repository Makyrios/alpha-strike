// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Pickups/AS_SpeedPickup.h"
#include "Characters/AS_Character.h"
#include "Components/AS_BuffComponent.h"

void AAS_SpeedPickup::PickupInteract(TArray<AActor*>& InteractedActors)
{
    Super::PickupInteract(InteractedActors);

    for (const AActor* InteractedActor : InteractedActors)
    {
        if (const AAS_Character* Character = Cast<AAS_Character>(InteractedActor))
        {
            if (UAS_BuffComponent* BuffComponent = Character->GetBuffComponent())
            {
                BuffComponent->ApplySpeedBuff(SpeedBuffMultiplier, SpeedBuffTime);
                Despawn();

                break;
            }
        }
    }
}
