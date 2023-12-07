// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "AS_Types.h"
#include "AS_TeamDeathmatchPlayerState.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_TeamDeathmatchPlayerState : public AAS_BasePlayerState
{
    GENERATED_BODY()

public:
    FORCEINLINE void SetTeam(ETeams NewTeam) { Team = NewTeam; }
    FORCEINLINE ETeams GetTeam() const { return Team; }
    FORCEINLINE void SetTeamColor(const FLinearColor& NewColor) { TeamColor = NewColor; }
    FORCEINLINE FLinearColor GetTeamColor() const { return TeamColor; }

protected:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AS|Stats")
    ETeams Team = ETeams::TEAM_A;

    FLinearColor TeamColor = FLinearColor::Black;
};
