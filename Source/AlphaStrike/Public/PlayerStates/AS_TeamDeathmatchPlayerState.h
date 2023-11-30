// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "AS_TeamDeathmatchPlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeams
{
    TEAM_A UMETA(DisplayName = "Team A"),
    TEAM_B UMETA(DisplayName = "Team B")
};

UCLASS()
class ALPHASTRIKE_API AAS_TeamDeathmatchPlayerState : public AAS_BasePlayerState
{
	GENERATED_BODY()
public:
    FORCEINLINE void SetTeam(ETeams NewTeam) { Team = NewTeam; }
    FORCEINLINE ETeams GetTeam() { return Team; }

protected:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AS|Stats")
    ETeams Team;
};
