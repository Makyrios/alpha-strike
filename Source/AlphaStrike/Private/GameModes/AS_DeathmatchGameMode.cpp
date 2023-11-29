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
    UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(this);
    if (!World || !NavigationSystem) return;

    FNavLocation NavLocation;
    bool FoundLocation;

    for (int i = 0; i < BotsSpawnInfo.NumberOfPawns; ++i)
    {
        FoundLocation = NavigationSystem->GetRandomPoint(NavLocation);
        if (FoundLocation)
        {
            APawn* NewPawn = World->SpawnActor<APawn>(BotsSpawnInfo.PawnClass, NavLocation.Location, FRotator(0.0f));
            SetBotsNames(NewPawn, i);
        }
    }
}

void AAS_DeathmatchGameMode::SetBotsNames(APawn* BotPawn, int32 BotIndex)
{
    if (!BotPawn) return;

    if (AAS_BasePlayerState* CustomPlayerState = BotPawn->GetPlayerState<AAS_BasePlayerState>())
    {
        FString BotName = FString("Bot ") + FString::FromInt(BotIndex); 
        CustomPlayerState->SetPlayerName(BotName);
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