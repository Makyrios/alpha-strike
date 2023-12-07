// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AS_BTService_CheckHealth.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTService_CheckHealth : public UBTService_BlackboardBase
{
    GENERATED_BODY()

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
