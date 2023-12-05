// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_BuffComponent.generated.h"

class AAS_Character;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALPHASTRIKE_API UAS_BuffComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAS_BuffComponent();

    void ApplySpeedBuff(float BuffMultiplier, float BuffTime);
    void ApplyJumpBuff(float BuffMultiplier, float BuffTime);

protected:
    virtual void BeginPlay() override;

private:
    void ResetSpeedBuff();
    void ResetJumpBuff();

private:
    AAS_Character* Character;

    FTimerHandle SpeedBuffHandle;
    FTimerHandle JumpBuffHandle;
};
