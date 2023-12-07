// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/AS_AIController.h"
#include "AI/AS_AICharacter.h"
#include "AI/Components/AS_AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AS_CombatComponent.h"

AAS_AIController::AAS_AIController()
{
    AS_AIPerceptionComponent = CreateDefaultSubobject<UAS_AIPerceptionComponent>("AS_AIPerceptionComponent");
    SetPerceptionComponent(*AS_AIPerceptionComponent);

    bWantsPlayerState = true;
}

void AAS_AIController::Aim()
{
    if (AS_AICharacter && AS_AICharacter->GetCombatComponent())
    {
        AS_AICharacter->GetCombatComponent()->Aim();
    }
}

void AAS_AIController::StopAim()
{
    if (AS_AICharacter && AS_AICharacter->GetCombatComponent())
    {
        AS_AICharacter->GetCombatComponent()->StopAim();
    }
}

void AAS_AIController::Fire()
{
    if (AS_AICharacter && AS_AICharacter->GetCombatComponent())
    {
        AS_AICharacter->GetCombatComponent()->Fire();
    }
}

void AAS_AIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    AS_AICharacter = Cast<AAS_AICharacter>(InPawn);
    if (AS_AICharacter)
    {
        RunBehaviorTree(AS_AICharacter->BehaviorTreeAsset);
    }
}

void AAS_AIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    SetFocusOnActor();
}

AActor* AAS_AIController::GetFocusOnActor() const
{
    if (!GetBlackboardComponent() || !GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName)) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}

void AAS_AIController::SetFocusOnActor() 
{
    AActor* AimActor = GetFocusOnActor();
    if (AimActor)
    {
        SetFocus(AimActor);
    }
}
