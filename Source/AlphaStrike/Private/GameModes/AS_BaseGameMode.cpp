// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_BaseGameMode.h"
#include "GameFramework/GameState.h"
#include "GameStates/AS_BaseGameState.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "AIController.h"
#include "Characters/AS_Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AS_GameInstance.h"
#include "Saves/AS_PlayerSavedInfo.h"
#include <UI/Widgets/AS_StartGameWidget.h>
#include <Controllers/AS_PlayerController.h>
#include "Components/AS_HealthComponent.h"

bool AAS_BaseGameMode::ReadyToStartMatch_Implementation()
{
    if (bDelayedStart)
    {
        return false;
    }

    if (GetMatchState() == MatchState::WaitingToStart)
    {
        return !GetWorldTimerManager().IsTimerActive(DelayStartTimer);
    }
    return false;
}

void AAS_BaseGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AAS_BasePlayerState* PlayerState = NewPlayer->GetPlayerState<AAS_BasePlayerState>();
    UAS_GameInstance* GameInstance = Cast<UAS_GameInstance>(GetGameInstance());
    if (GameInstance)
    {
        FString PlayerName = GameInstance->GetPlayerName().ToString();
        PlayerState->SetPlayerName(PlayerName);
    }

    CreateStartGameWidget(NewPlayer);

    PlayerControllerList.Add(NewPlayer);
}

void AAS_BaseGameMode::CreateStartGameWidget(APlayerController* NewPlayer)
{
    if (AAS_PlayerController* CustomPlayerController = Cast<AAS_PlayerController>(NewPlayer))
    {
        float CurrentDelayBeforeStart = DelayBeforeStart - GameState->GetServerWorldTimeSeconds();
        CustomPlayerController->CreateStartGameWidget(CurrentDelayBeforeStart);
    }
}

void AAS_BaseGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    if (PlayerControllerList.Num() > RespawnPoints.Num()) return;
    for (int i = 0; i < PlayerControllerList.Num(); ++i)
    {
        APawn* Pawn = PlayerControllerList[i]->GetPawn();
        if (Pawn)
        {
            Pawn->SetActorLocation(RespawnPoints[i]);
        }
    }
}

void AAS_BaseGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        if (PlayerStarts[i])
        {
            RespawnPoints.Add(PlayerStarts[i]->GetActorLocation());
        }
    }

    if (bDelayedStart)
    {
        GetWorldTimerManager().SetTimer(DelayStartTimer, this, &AAS_BaseGameMode::StartMatch, DelayBeforeStart);
    }
}

void AAS_BaseGameMode::HandleActorDeath(AController* DeadActor, AController* KillerActor)
{
    if (!DeadActor || !KillerActor) return;

    FTimerHandle DelayRespawnTimer;
    RespawnDelegate.BindUFunction(this, FName("RespawnPawn"), DeadActor);
    GetWorldTimerManager().SetTimer(DelayRespawnTimer, RespawnDelegate, MinRespawnDelay, false, MinRespawnDelay);

    AddKillsAndDeathsToPlayers(DeadActor, KillerActor);
}

void AAS_BaseGameMode::AddKillsAndDeathsToPlayers(AController* DeadActor, AController* KillerActor)
{
    auto DeadActorPlayerState = DeadActor->GetPlayerState<AAS_BasePlayerState>();
    auto KillerActorPlayerState = KillerActor->GetPlayerState<AAS_BasePlayerState>();

    if (!DeadActorPlayerState || !KillerActorPlayerState) return;

    DeadActorPlayerState->AddDeath();
    KillerActorPlayerState->AddKill();
}

void AAS_BaseGameMode::RespawnPawn(AController* Controller)
{
    UWorld* World = GetWorld();
    APawn* OldPawn = Controller->GetPawn();
    AActor* PlayerStart = FindPlayerStart(Controller);

    if (!World || !Controller || !OldPawn || !PlayerStart) return;

    APawn* NewPawn = World->SpawnActor<APawn>(OldPawn->GetClass(), PlayerStart->GetActorLocation(), FRotator(0.0f));
    OldPawn->Destroy();
    if (NewPawn)
    {
        Controller->UnPossess();
        Controller->Possess(NewPawn);

        if (bInvincibleOnSpawn)
        {
            MakeInvincible(NewPawn);
        }
    }
}

void AAS_BaseGameMode::MakeInvincible(APawn* NewPawn)
{
    if (AAS_Character* SpawnedCharacter = Cast<AAS_Character>(NewPawn))
    {
        SpawnedCharacter->GetHealthComponent()->SetInvincible(true, InvincibilityTime);
    }
}

bool AAS_BaseGameMode::ReadyToEndMatch_Implementation()
{
    if (AAS_BaseGameState* CurrentGameState = Cast<AAS_BaseGameState>(GameState))
    {
        return CurrentGameState->ElapsedTime >= TimeLimit;
    }
    return false;
}

void AAS_BaseGameMode::HandleMatchHasEnded()
{
    Super::HandleMatchHasEnded();

    UE_LOG(LogTemp, Warning, TEXT("Match ended!"));
}
