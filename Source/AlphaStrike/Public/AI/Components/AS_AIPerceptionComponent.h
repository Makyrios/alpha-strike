// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AS_AIPerceptionComponent.generated.h"

class AAS_BaseGameMode;
class AAS_TeamDeathmatchPlayerState;

UCLASS()
class ALPHASTRIKE_API UAS_AIPerceptionComponent : public UAIPerceptionComponent
{
    GENERATED_BODY()

public:
    AActor* GetClosestEnemy();

private:
    UPROPERTY()
    AAS_BaseGameMode* AS_BaseGameMode;

    UPROPERTY()
    AAS_TeamDeathmatchPlayerState* AS_TeamDeathmatchPlayerState;
};
