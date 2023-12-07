// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "AS_BTService_CheckDistanceToTarget.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTService_CheckDistanceToTarget : public UBTService_BlackboardBase
{
    GENERATED_BODY()

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    FBlackboardKeySelector bTargetInFireRangeKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    float FireRange = 3000;
};
