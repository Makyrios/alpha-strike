﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Characters/AS_Character.h"
#include "AS_AICharacter.generated.h"

class UBehaviorTree;

UCLASS()
class ALPHASTRIKE_API AAS_AICharacter : public AAS_Character
{
    GENERATED_BODY()

public:
    AAS_AICharacter();

public:
    UPROPERTY(EditDefaultsOnly, Category = "AS|AI")
    UBehaviorTree* BehaviorTreeAsset;
};
