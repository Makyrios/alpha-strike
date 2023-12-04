// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_TeamDeathmatchGameMode.h"
#include "NavigationSystem.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"
#include "GameStates/AS_TeamDeathmatchGameState.h"
#include "AS_PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/AS_Character.h"
#include "Controllers/AS_PlayerController.h"

void AAS_TeamDeathmatchGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    if (bSpawnBots)
    {
        AAS_TeamDeathmatchPlayerState* TeamPlayerState = PlayerControllerList[0]->GetPlayerState<AAS_TeamDeathmatchPlayerState>();
        if (TeamPlayerState)
        {
            TeamPlayerState->SetTeamColor(TeamsSpawnInfo[TeamPlayerState->GetTeam()].TeamColor);
            TeamsSpawnInfo[TeamPlayerState->GetTeam()].NumberOfPawns -= 1;
        }
        SpawnBotsPawns(ETeams::TEAM_A);
        SpawnBotsPawns(ETeams::TEAM_B);
        UpdateTeamsScoreInHUDs();
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
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            TSubclassOf<APawn> RandPawnClass = (FMath::RandBool()) ? TeamSpawnInfo.PawnClass : TeamSpawnInfo.HeavyPawnClass;

            AAS_Character* Pawn = World->SpawnActor<AAS_Character>(
                RandPawnClass, SpawnActor->GetActorLocation(), SpawnActor->GetActorRotation(), SpawnParameters);
            if (Pawn)
            {
                Pawn->SetPlayerColor(TeamSpawnInfo.TeamColor);
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

    if (AAS_TeamDeathmatchPlayerState* TeamPlayerState = Player->GetPlayerState<AAS_TeamDeathmatchPlayerState>())
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

    TArray<AAS_PlayerStart*> TeamPlayerStarts;
    for (auto PlayerStart : PlayerStarts)
    {
        AAS_PlayerStart* CustomPlayerStart = Cast<AAS_PlayerStart>(PlayerStart);
        if (CustomPlayerStart->GetTeam() == TeamPlayerState->GetTeam() && !CustomPlayerStart->GetIsOccupied())
        {
            TeamPlayerStarts.AddUnique(CustomPlayerStart);
        }
    }

    if (TeamPlayerStarts.IsEmpty()) return nullptr;

    const int32 RandIndex = FMath::RandRange(0, TeamPlayerStarts.Num() - 1);
    TeamPlayerStarts[RandIndex]->SetIsOccupied(true);

    return TeamPlayerStarts[RandIndex];
}

void AAS_TeamDeathmatchGameMode::HandleActorDeath(
    AController* DeadActor, AController* KillerActor, bool bEnableRandColor, const FLinearColor& CustomColor)
{
    AAS_TeamDeathmatchPlayerState* TeamPlayerState = DeadActor->GetPlayerState<AAS_TeamDeathmatchPlayerState>();
    if (!TeamPlayerState || TeamsSpawnInfo.IsEmpty()) return;

    FTeamSpawnInfo TeamSpawnInfo = TeamsSpawnInfo[TeamPlayerState->GetTeam()];

    if (AAS_TeamDeathmatchGameState* CurrentGameState = GetGameState<AAS_TeamDeathmatchGameState>())
    {
        CurrentGameState->AddScoreToTeam(KillerActor);
        UpdateTeamsScoreInHUDs();
    }

    Super::HandleActorDeath(DeadActor, KillerActor, false, TeamSpawnInfo.TeamColor);
}

void AAS_TeamDeathmatchGameMode::UpdateTeamsScoreInHUDs()
{
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        AAS_PlayerController* PlayerController = Cast<AAS_PlayerController>(*Iterator);
        if (PlayerController)
        {
            PlayerController->SetTeamsScore();
        }
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
