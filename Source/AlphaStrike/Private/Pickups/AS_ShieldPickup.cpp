// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Pickups/AS_ShieldPickup.h"
#include "Characters/AS_Character.h"
#include "Components/AS_HealthComponent.h"

void AAS_ShieldPickup::PickupInteract(TArray<AActor*>& InteractedActors)
{
    Super::PickupInteract(InteractedActors);

    for (const AActor* InteractedActor : InteractedActors)
    {
        if (const AAS_Character* Character = Cast<AAS_Character>(InteractedActor))
        {
            if (UAS_HealthComponent* HealthComponent = Character->GetHealthComponent())
            {
                if (HealthComponent->GetShieldPercent() < 1.f)
                {
                    HealthComponent->AddShield(ShieldAmount);
                    Despawn();

                    break;
                }
            }
        }
    }
}
