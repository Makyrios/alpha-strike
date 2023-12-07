// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameStates/AS_BaseGameState.h"
#include "AS_DeathmatchGameState.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_DeathmatchGameState : public AAS_BaseGameState
{
    GENERATED_BODY()

public:
    FORCEINLINE int32 GetMaxKills() const { return MaxPlayerKills; }
    FORCEINLINE AController* GetWinningPlayer() const { return WinningPlayer; }
    FORCEINLINE int32 GetMaxPlayerKills() const { return MaxPlayerKills; }

    void UpdateMaxKillsAndWinningPlayer(AController* PlayerController);

protected:
    UPROPERTY()
    AController* WinningPlayer;

    int32 MaxPlayerKills;
};
