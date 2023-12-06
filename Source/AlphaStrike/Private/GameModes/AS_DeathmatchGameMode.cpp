// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_DeathmatchGameMode.h"
#include "GameStates/AS_DeathmatchGameState.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "NavigationSystem.h"
#include "Characters/AS_Character.h"
#include "Controllers/AS_PlayerController.h"
#include "Kismet/GameplayStatics.h"

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

            AAS_Character* Pawn =
                World->SpawnActor<AAS_Character>(RandPawnClass, PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

            Pawn->SetPlayerColor(FLinearColor::MakeRandomColor());
            Controller->Possess(Pawn);
            SetBotName(Controller, i);
        }
    }
}

void AAS_DeathmatchGameMode::HandleActorDeath(
    AController* DeadActor, AController* KillerActor, bool bEnableRandColor, const FLinearColor& CustomColor)
{
    Super::HandleActorDeath(DeadActor, KillerActor);

    if (AAS_DeathmatchGameState* CurrentGameState = GetGameState<AAS_DeathmatchGameState>())
    {
        CurrentGameState->UpdateMaxKillsAndWinningPlayer(KillerActor);
    }
}

bool AAS_DeathmatchGameMode::ReadyToEndMatch_Implementation()
{
    if (AAS_DeathmatchGameState* CurrentGameState = Cast<AAS_DeathmatchGameState>(GameState))
    {
        return CurrentGameState->GetMaxPlayerKills() >= ScoreGoal || Super::ReadyToEndMatch_Implementation();
    }
    return false;
}

void AAS_DeathmatchGameMode::HandleMatchHasEnded()
{
    Super::HandleMatchHasEnded();

    AAS_DeathmatchGameState* CustomGameState = GetGameState<AAS_DeathmatchGameState>();
    if (!CustomGameState) return;

    AController* WonController = CustomGameState->GetWinningPlayer();
    if (!WonController) return;

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        AAS_PlayerController* CustomController = Cast<AAS_PlayerController>(*Iterator);
        if (CustomController && (*Iterator).Get() == WonController)
        {
            CustomController->HandleWin();
        }
        else
        {
            CustomController->HandleLose();
        }
    }
}
