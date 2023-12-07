// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "AS_BTTask_MoveToInFiringRange.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTTask_MoveToInFiringRange : public UBTTask_MoveTo
{
    GENERATED_BODY()

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
