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
#include "UI/Widgets/AS_StartGameWidget.h"
#include "Controllers/AS_PlayerController.h"
#include "Components/AS_HealthComponent.h"

void AAS_BaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    if (bDelayedStart)
    {
        GetWorldTimerManager().SetTimer(DelayStartTimer, this, &AAS_BaseGameMode::StartMatch, DelayBeforeStart);
    }
}

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
        const FString PlayerName = GameInstance->GetPlayerName().ToString();
        PlayerState->SetPlayerName(PlayerName);
    }
    CreateStartGameWidget(NewPlayer);
}

void AAS_BaseGameMode::CreateStartGameWidget(APlayerController* NewPlayer)
{
    if (AAS_PlayerController* CustomPlayerController = Cast<AAS_PlayerController>(NewPlayer))
    {
        if (!IsGameStarted())
        {
            const float CurrentDelayBeforeStart = DelayBeforeStart - GameState->GetServerWorldTimeSeconds();
            CustomPlayerController->CreateStartGameWidget(CurrentDelayBeforeStart);
        }
    }
}

void AAS_BaseGameMode::HandleMatchHasStarted()
{
    Super::HandleMatchHasStarted();

    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        const AActor* PlayerStart = ChoosePlayerStart_Implementation(Iterator->Get());
        APawn* PlayerPawn = (*Iterator)->GetPawn();
        if (PlayerStart && PlayerPawn)
        {
            PlayerPawn->SetActorLocation(PlayerStart->GetActorLocation());
            PlayerPawn->SetActorRotation(PlayerStart->GetActorRotation());
        }
        if (AAS_PlayerController* CustomPlayerController = Cast<AAS_PlayerController>(*Iterator))
        {
            CustomPlayerController->SetHUDWidgetVisibility(ESlateVisibility::Visible);
            CustomPlayerController->SetScoreGoal(ScoreGoal);
        }
    }
}

AActor* AAS_BaseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (!Player) return nullptr;

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, AAS_PlayerStart::StaticClass(), PlayerStarts);

    TArray<AAS_PlayerStart*> ValidPlayerStarts;
    for (int i = 0; i < PlayerStarts.Num(); ++i)
    {
        AAS_PlayerStart* CustomPlayerStart = Cast<AAS_PlayerStart>(PlayerStarts[i]);
        if (!(CustomPlayerStart->GetIsOccupied()))
        {
            ValidPlayerStarts.AddUnique(CustomPlayerStart);
        }
    }

    if (ValidPlayerStarts.IsEmpty()) return nullptr;

    const int32 RandIndex = FMath::RandRange(0, ValidPlayerStarts.Num() - 1);
    ValidPlayerStarts[RandIndex]->SetIsOccupied(true);

    return ValidPlayerStarts[RandIndex];
}

void AAS_BaseGameMode::SetBotName(AController* BotController, int32 BotIndex)
{
    if (!BotController) return;

    if (AAS_BasePlayerState* CustomPlayerState = BotController->GetPlayerState<AAS_BasePlayerState>())
    {
        const FString BotName = FString("Bot ") + FString::FromInt(BotIndex);
        CustomPlayerState->SetPlayerName(BotName);
    }
}

void AAS_BaseGameMode::HandleActorDeath(
    AController* DeadActor, AController* KillerActor, bool bEnableRandColor, const FLinearColor& CustomColor)
{
    if (!DeadActor || !KillerActor) return;

    FTimerHandle DelayRespawnTimer;
    RespawnDelegate.BindUFunction(this, FName("RespawnPawn"), DeadActor, bEnableRandColor, CustomColor);
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

void AAS_BaseGameMode::RespawnPawn(AController* Controller, bool bEnableRandColor, const FLinearColor& CustomColor)
{
    UWorld* World = GetWorld();
    APawn* OldPawn = Controller->GetPawn();
    const AActor* PlayerStart = ChoosePlayerStart_Implementation(Controller);

    if (!World || !Controller || !OldPawn || !PlayerStart) return;

    AAS_Character* NewPawn =
        World->SpawnActor<AAS_Character>(OldPawn->GetClass(), PlayerStart->GetActorLocation(), PlayerStart->GetActorRotation());

    OldPawn->Destroy();

    if (NewPawn)
    {
        Controller->UnPossess();
        Controller->Possess(NewPawn);

        if (bEnableRandColor)
        {
            NewPawn->SetPlayerColor(FLinearColor::MakeRandomColor());
        }
        else
        {
            NewPawn->SetPlayerColor(CustomColor);
        }

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
        if (SpawnedCharacter->GetHealthComponent())
        {
            SpawnedCharacter->GetHealthComponent()->SetInvincible(true, InvincibilityTime);
        }
    }
}

bool AAS_BaseGameMode::ReadyToEndMatch_Implementation()
{
    if (const AAS_BaseGameState* CurrentGameState = Cast<AAS_BaseGameState>(GameState))
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

    const FString MapName = UGameplayStatics::GetCurrentLevelName(World);
    World->ServerTravel(MapName);
}

bool AAS_BaseGameMode::IsGameStarted()
{
    UWorld* World = GetWorld();
    if (!World) return false;

    return !(World->GetTimerManager().IsTimerActive(DelayStartTimer));
}
