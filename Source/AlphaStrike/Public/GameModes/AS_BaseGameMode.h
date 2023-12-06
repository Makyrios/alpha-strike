// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AS_BaseGameMode.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_BaseGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    virtual void HandleActorDeath(AController* DeadActor, AController* KillerActor, bool bEnableRandColor = true,
        const FLinearColor& CustomColor = FLinearColor::Black);

    FORCEINLINE float GetTimeLimit() const { return TimeLimit; }
    FORCEINLINE float GetDelayBeforeStart() const { return DelayBeforeStart; }
    virtual bool IsGameStarted();

protected:
    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual bool ReadyToStartMatch_Implementation() override;

    virtual void HandleMatchHasStarted() override;
    AActor* ChoosePlayerStart_Implementation(AController* Player) override;
    virtual void SetBotName(AController* BotController, int32 BotIndex);

    UFUNCTION()
    virtual void RespawnPawn(AController* Controller, bool bEnableRandColor = true, const FLinearColor& CustomColor = FLinearColor::Black);

    void MakeInvincible(APawn* NewPawn);

    virtual bool ReadyToEndMatch_Implementation() override;
    virtual void HandleMatchHasEnded() override;

    void CreateStartGameWidget(APlayerController* NewPlayer);

    virtual void BeginPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|GameStart", meta = (EditCondition = "bDelayedStart"))
    float DelayBeforeStart;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    int32 ScoreGoal;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    float TimeLimit;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay")
    bool bInvincibleOnSpawn = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Gameplay", meta = (EditCondition = "bInvincibleOnSpawn"))
    float InvincibilityTime = 2.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|GameEnd")
    float DelayBeforeRestart;

    FTimerHandle DelayStartTimer;

    FTimerDelegate RespawnDelegate;

private:
    void AddKillsAndDeathsToPlayers(AController* DeadActor, AController* KillerActor);
    void RestartGame();
};
