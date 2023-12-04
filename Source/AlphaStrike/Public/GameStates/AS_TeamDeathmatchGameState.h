// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameStates/AS_BaseGameState.h"
#include "AS_TeamDeathmatchGameState.generated.h"

enum class ETeams;

USTRUCT(BlueprintType)
struct FTeamInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<AController*> Members;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Score;
};

UCLASS()
class ALPHASTRIKE_API AAS_TeamDeathmatchGameState : public AAS_BaseGameState
{
	GENERATED_BODY()

public:
    AAS_TeamDeathmatchGameState();

    int32 GetTeamScore(ETeams Team) const;
    TArray<AController*> GetWinningTeamPlayers();

    void AddPlayerToTeam(AController* Player, ETeams Team);

    void AddScoreToTeam(AController* ScoredPlayer);

protected:
    TMap<ETeams, FTeamInfo> TeamsInfo;
};
