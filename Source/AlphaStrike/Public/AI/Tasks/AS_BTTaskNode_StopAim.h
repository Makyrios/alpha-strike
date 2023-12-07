// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AS_BTTaskNode_StopAim.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTTaskNode_StopAim : public UBTTaskNode
{
    GENERATED_BODY()

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
