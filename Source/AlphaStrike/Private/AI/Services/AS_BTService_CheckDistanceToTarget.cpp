// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Services/AS_BTService_CheckDistanceToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AS_AICharacter.h"
#include "AI/AS_AIController.h"
#include "Components/AS_CombatComponent.h"
#include "Weapons/AS_BaseWeapon.h"

void UBTService_CheckDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard || !Blackboard->GetValueAsObject(TargetKey.SelectedKeyName)) return;

    AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!Target || !OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn()) return;

    if (AAS_AICharacter* AICharacter = Cast<AAS_AICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
    {
        if (AICharacter->GetCombatComponent())
        {
            if (AAS_BaseWeapon* EquippedWeapon = AICharacter->GetCombatComponent()->GetEquippedWeapon())
            {
                if (FVector::Dist(AICharacter->GetActorLocation(), Target->GetActorLocation()) <= FireRange)
                {
                    Blackboard->SetValueAsBool(bTargetInFireRangeKey.SelectedKeyName, true);
                }
                else
                {
                    Blackboard->SetValueAsBool(bTargetInFireRangeKey.SelectedKeyName, false);
                }
            }
        }
    }
}
