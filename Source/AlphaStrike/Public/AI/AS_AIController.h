﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AS_AIController.generated.h"

class UAS_AIPerceptionComponent;
class AAS_AICharacter;

UCLASS()
class ALPHASTRIKE_API AAS_AIController : public AAIController
{
    GENERATED_BODY()

public:
    AAS_AIController();

    void Fire();

    void Aim();
    void StopAim();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AS|AI")
    UAS_AIPerceptionComponent* AS_AIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|AI")
    FName FocusOnKeyName = "EnemyActor";

    AAS_AICharacter* AS_AICharacter;

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

private:
    AActor* GetFocusOnActor() const;
    void SetFocusOnActor();
};
