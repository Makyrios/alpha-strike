// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "AS_NeedHealthDecorator.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_NeedHealthDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    UAS_NeedHealthDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MinPercentForHeal = 0.5;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
