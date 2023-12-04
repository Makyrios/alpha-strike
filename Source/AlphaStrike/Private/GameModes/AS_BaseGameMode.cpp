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
#include "AS_PlayerStart.h"
#include "Saves/AS_PlayerSavedInfo.h"
#include "UI/Widgets/AS_StartGameWidget.h"
#include "Controllers/AS_PlayerController.h"
#include "Components/AS_HealthComponent.h"

bool AAS_BaseGameMode::ReadyToStartMatch_Implementation()
{
    if (bDelayedStart) return false;

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
    if (GameInstance && PlayerState)
    {
        FString PlayerName = GameInstance->GetPlayerName().ToString();
        PlayerState->SetPlayerName(PlayerName);
    }

    CreateStartGameWidget(NewPlayer);
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

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        AActor* PlayerStart = ChoosePlayerStart_Implementation(Iterator->Get());
        APawn* PlayerPawn = (*Iterator)->GetPawn();
        if (PlayerStart && PlayerPawn)
        {
            PlayerPawn->SetActorLocation(PlayerStart->GetActorLocation());
            PlayerPawn->SetActorRotation(PlayerStart->GetActorRotation());
        }
    }
}

AActor* AAS_BaseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return nullptr;

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, AAS_PlayerStart::StaticClass(), PlayerStarts);
    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        AAS_PlayerStart* CustomPlayerStart = Cast<AAS_PlayerStart>(PlayerStarts[i]);
        if (!(CustomPlayerStart->GetIsOccupied()))
        {
            CustomPlayerStart->SetIsOccupied(true);
            return CustomPlayerStart;
        }
    }
    return nullptr;
}

void AAS_BaseGameMode::SetBotName(AController* BotController, int32 BotIndex)
{
    if (!BotController) return;

    if (AAS_BasePlayerState* CustomPlayerState = BotController->GetPlayerState<AAS_BasePlayerState>())
    {
        FString BotName = FString("Bot ") + FString::FromInt(BotIndex);
        CustomPlayerState->SetPlayerName(BotName);
    }
}

void AAS_BaseGameMode::BeginPlay()
{
    Super::BeginPlay();

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
    GetWorldTimerManager().SetTimer(DelayRespawnTimer, RespawnDelegate, MinRespawnDelay, false);

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
    AActor* PlayerStart = ChoosePlayerStart_Implementation(Controller);

    if (!World || !Controller || !OldPawn || !PlayerStart) return;

    APawn* NewPawn = World->SpawnActor<APawn>(OldPawn->GetClass(), PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());
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
    
    UWorld* World = GetWorld();
    if (!World) return;

    UGameplayStatics::SetGlobalTimeDilation(this, 0.3);
    FTimerHandle OpenLevelTimer;
    FTimerDelegate OpenLevelDelegate;
    OpenLevelDelegate.BindUObject(this, &AAS_BaseGameMode::RestartGame);

    World->GetTimerManager().SetTimer(OpenLevelTimer, OpenLevelDelegate, DelayBeforeRestart, false);
}

void AAS_BaseGameMode::RestartGame() 
{
    UWorld* World = GetWorld();
    if (!World) return;

    UGameplayStatics::OpenLevelBySoftObjectPtr(this, GetWorld()->GetCurrentLevel());
}
