// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "AS_Types.h"
#include "AS_PlayerStart.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_PlayerStart : public APlayerStart
{
    GENERATED_BODY()
public:
    void SetIsOccupied(bool IsOccupied);
    FORCEINLINE bool GetIsOccupied() { return bIsOccupied; }

    FORCEINLINE ETeams GetTeam() { return Team; }

private:
    UPROPERTY(EditAnywhere, Category = "AS|Spawn")
    float OccupiedTime;

    UPROPERTY(EditAnywhere, Category = "AS|Spawn")
    ETeams Team;

    bool bIsOccupied;

    FTimerHandle OccupiedTimer;
};
