// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "AS_BaseGameState.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_BaseGameState : public AGameState
{
    GENERATED_BODY()

public:
    AAS_BaseGameState();
    virtual void Tick(float DeltaTime) override;

protected:
    void SetTimeForHUD();
};
