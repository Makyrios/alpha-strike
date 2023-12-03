// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_DeathmatchGameMode.h"
#include "GameStates/AS_DeathmatchGameState.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "NavigationSystem.h"

void AAS_DeathmatchGameMode::HandleMatchHasStarted() 
{
    Super::HandleMatchHasStarted();

    if (bSpawnBots)
    {
        SpawnBotsPawns();
    }
}

void AAS_DeathmatchGameMode::SpawnBotsPawns()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (int i = 0; i < BotsSpawnInfo.NumberOfPawns; ++i)
    {
        AController* Controller = World->SpawnActor<AController>(BotsSpawnInfo.ControllerClass);
        AActor* PlayerStart = ChoosePlayerStart_Implementation(Controller);
        if (Controller && PlayerStart)
        {
            TSubclassOf<APawn> RandPawnClass = (FMath::RandBool()) ? BotsSpawnInfo.PawnClass : BotsSpawnInfo.HeavyPawnClass;

            APawn* Pawn = World->SpawnActor<APawn>(RandPawnClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
            Controller->Possess(Pawn);
            SetBotName(Controller, i);
        }
    }
}


void AAS_DeathmatchGameMode::HandleActorDeath(AController* DeadActor, AController* KillerActor) 
{
    Super::HandleActorDeath(DeadActor, KillerActor);

    if (AAS_DeathmatchGameState* CurrentGameState = GetGameState<AAS_DeathmatchGameState>())
    {
        CurrentGameState->UpdateMaxKillsAndWinningPlayer(KillerActor);
    }
}

bool AAS_DeathmatchGameMode::ReadyToEndMatch_Implementation() {
    if (AAS_DeathmatchGameState* CurrentGameState = Cast<AAS_DeathmatchGameState>(GameState))
    {
        return CurrentGameState->GetMaxPlayerKills() >= KillsToWin || Super::ReadyToEndMatch_Implementation();
    }
    return false;
}
