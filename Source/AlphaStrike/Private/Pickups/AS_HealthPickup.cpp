// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Pickups/AS_HealthPickup.h"
#include "Characters/AS_Character.h"
#include "Components/AS_HealthComponent.h"

void AAS_HealthPickup::PickupInteract(TArray<AActor*>& InteractedActors)
{
    Super::PickupInteract(InteractedActors);

    for (const AActor* InteractedActor : InteractedActors)
    {
        if (const AAS_Character* Character = Cast<AAS_Character>(InteractedActor))
        {
            if (UAS_HealthComponent* HealthComponent = Character->GetHealthComponent())
            {
                if (HealthComponent->GetHealthPercent() < 1.f)
                {
                    HealthComponent->AddHealth(HealAmount);
                    Despawn();

                    break;
                }
            }
        }
    }
}
