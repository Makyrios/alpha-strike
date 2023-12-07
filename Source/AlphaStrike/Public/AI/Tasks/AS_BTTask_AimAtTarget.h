// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AS_BTTask_AimAtTarget.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTTask_AimAtTarget : public UBTTaskNode
{
    GENERATED_BODY()

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
