// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameModes/AS_BaseGameMode.h"
#include "AS_Types.h"
#include "AS_TeamDeathmatchGameMode.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_TeamDeathmatchGameMode : public AAS_BaseGameMode
{
    GENERATED_BODY()

public:
    virtual void HandleActorDeath(AController* DeadActor, AController* KillerActor, bool bEnableRandColor = true,
        const FLinearColor& CustomColor = FLinearColor::Black) override;

protected:
    virtual void HandleMatchHasStarted() override;
    void SpawnBotsPawns(ETeams TeamToSpawn);
    AActor* ChoosePlayerStart_Implementation(AController* Player) override;
    void GiveTeamToPlayer(AController* Pawn, ETeams TeamToGive);

    void UpdateTeamsScoreInHUDs();

    virtual bool ReadyToEndMatch_Implementation() override;
    virtual void HandleMatchHasEnded() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|GameStart", meta = (DisplayName = "SpawnBots", InlineEditConditionToggle))
    bool bSpawnBots;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AS|GameStart", meta = (EditCondition = "bSpawnBots"))
    TMap<ETeams, FTeamSpawnInfo> TeamsSpawnInfo;
};
