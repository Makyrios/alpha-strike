// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_TeamDeathmatchGameMode.h"
#include "NavigationSystem.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"
#include "GameStates/AS_TeamDeathmatchGameState.h"
#include "AS_PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AAS_TeamDeathmatchGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    if (bSpawnBots)
    {
        AAS_TeamDeathmatchPlayerState* TeamPlayerState = PlayerControllerList[0]->GetPlayerState<AAS_TeamDeathmatchPlayerState>();
        if (TeamPlayerState)
        {
            TeamsSpawnInfo[TeamPlayerState->GetTeam()].NumberOfPawns -= 1;
        }
        SpawnBotsPawns(ETeams::TEAM_A);
        SpawnBotsPawns(ETeams::TEAM_B);
    }
}

void AAS_TeamDeathmatchGameMode::SpawnBotsPawns(ETeams TeamToSpawn)
{
    FTeamSpawnInfo TeamSpawnInfo = TeamsSpawnInfo[TeamToSpawn];
    UWorld* World = GetWorld();

    if (!World) return;

    for (int i = 0; i < TeamSpawnInfo.NumberOfPawns; ++i)
    {
        AController* Controller = World->SpawnActor<AController>(TeamSpawnInfo.ControllerClass);
        GiveTeamToPlayer(Controller, TeamToSpawn);
        AActor* SpawnActor = ChoosePlayerStart_Implementation(Controller);
        if (Controller && SpawnActor)
        {
            APawn* Pawn = World->SpawnActor<APawn>(TeamSpawnInfo.PawnClass, SpawnActor->GetActorLocation(), SpawnActor->GetActorRotation());
            if (Pawn)
            {
                SetBotName(Controller, i);
                Controller->Possess(Pawn);
            }
        }
    }
}

void AAS_TeamDeathmatchGameMode::GiveTeamToPlayer(AController* Player, ETeams TeamToGive)
{
    AAS_TeamDeathmatchGameState* CurrentGameState = GetGameState<AAS_TeamDeathmatchGameState>();
    if (!Player || !CurrentGameState) return;

    if (AAS_TeamDeathmatchPlayerState* TeamPlayerState = Player->GetPlayerState < AAS_TeamDeathmatchPlayerState>())
    {
        TeamPlayerState->SetTeam(TeamToGive);
        CurrentGameState->AddPlayerToTeam(TeamPlayerState, TeamToGive);
    }
}

AActor* AAS_TeamDeathmatchGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return nullptr;

    TArray<AActor*> PlayerStarts;
    AAS_TeamDeathmatchPlayerState* TeamPlayerState = Player->GetPlayerState<AAS_TeamDeathmatchPlayerState>();
    UGameplayStatics::GetAllActorsOfClass(this, AAS_PlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.IsEmpty() || !TeamPlayerState) return nullptr;

    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        AAS_PlayerStart* CustomPlayerStart = Cast<AAS_PlayerStart>(PlayerStarts[i]);
        if (CustomPlayerStart->GetTeam() == TeamPlayerState->GetTeam() && !(CustomPlayerStart->GetIsOccupied()))
        {
            CustomPlayerStart->SetIsOccupied(true);
            return CustomPlayerStart;
        }
    }
    return nullptr;
 }

void AAS_TeamDeathmatchGameMode::HandleActorDeath(AController* DeadActor, AController* KillerActor)
{
    Super::HandleActorDeath(DeadActor, KillerActor);

    if (AAS_TeamDeathmatchGameState* CurrentGameState = GetGameState<AAS_TeamDeathmatchGameState>())
    {
        CurrentGameState->AddScoreToTeam(KillerActor);
    }
}

bool AAS_TeamDeathmatchGameMode::ReadyToEndMatch_Implementation()
{
    if (AAS_TeamDeathmatchGameState* CurrentGameState = GetGameState<AAS_TeamDeathmatchGameState>())
    {
        return CurrentGameState->GetTeamScore(ETeams::TEAM_A) >= ScoreGoal || CurrentGameState->GetTeamScore(ETeams::TEAM_B) >= ScoreGoal ||
               Super::ReadyToEndMatch_Implementation();
    }
    return false;
}
