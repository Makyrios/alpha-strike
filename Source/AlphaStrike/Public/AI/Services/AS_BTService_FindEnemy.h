// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AS_BTService_FindEnemy.generated.h"

UCLASS()
class ALPHASTRIKE_API UBTService_FindEnemy : public UBTService
{
    GENERATED_BODY()

public:
    UBTService_FindEnemy();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    FBlackboardKeySelector EnemyActorKey;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    FBlackboardKeySelector AimLocationKey;
};
