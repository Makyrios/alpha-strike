// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Components/AS_AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "AIController.h"
#include "GameModes/AS_DeathmatchGameMode.h"
#include "GameModes/AS_TeamDeathmatchGameMode.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"

AActor* UAS_AIPerceptionComponent::GetClosestEnemy()
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

    if (PercieveActors.Num() == 0)
    {
        GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PercieveActors);
        if (PercieveActors.Num() == 0)
        {
            return nullptr;
        }
    }

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for (const auto PercieveActor : PercieveActors)
    {
        const auto PercievePawn = Cast<APawn>(PercieveActor);
        if (!PercievePawn) return nullptr;

        AS_BaseGameMode = (!AS_BaseGameMode) ? GetWorld()->GetAuthGameMode<AAS_DeathmatchGameMode>() : AS_BaseGameMode;
        if (!AS_BaseGameMode)
        {
            AS_BaseGameMode = (!AS_BaseGameMode) ? GetWorld()->GetAuthGameMode<AAS_TeamDeathmatchGameMode>() : AS_BaseGameMode;
            if (!AS_BaseGameMode) return nullptr;
        }

        AS_TeamDeathmatchPlayerState =
            (!AS_TeamDeathmatchPlayerState) ? Controller->GetPlayerState<AAS_TeamDeathmatchPlayerState>() : AS_TeamDeathmatchPlayerState;
        if (!AS_TeamDeathmatchPlayerState) return nullptr;

        bool AreFriends = true;

        if (AS_BaseGameMode->IsA<AAS_DeathmatchGameMode>())
        {
            AAIController* PercieveController = PercievePawn->GetController<AAIController>();
            AreFriends = (!PercieveController) ? false : true;
        }
        else if (AS_BaseGameMode->IsA<AAS_TeamDeathmatchGameMode>())
        {
            const auto PercievePlayerState = PercievePawn->GetPlayerState<AAS_TeamDeathmatchPlayerState>();
            if (!PercievePlayerState) return nullptr;

            AreFriends = AS_TeamDeathmatchPlayerState->GetTeam() == PercievePlayerState->GetTeam();
        }

        if (!AreFriends)
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}
