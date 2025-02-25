// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "AS_EnvQueryTest_PickupCouldBeTaken.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_EnvQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
    GENERATED_BODY()

public:
    UAS_EnvQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

    virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
