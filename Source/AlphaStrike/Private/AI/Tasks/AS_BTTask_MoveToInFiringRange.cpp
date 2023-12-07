// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Tasks/AS_BTTask_MoveToInFiringRange.h"
#include "AI/AS_AICharacter.h"
#include "AIController.h"
#include "Components/AS_CombatComponent.h"
#include "Weapons/AS_BaseWeapon.h"

EBTNodeResult::Type UBTTask_MoveToInFiringRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn()) return EBTNodeResult::Failed;

    if (AAS_AICharacter* AICharacter = Cast<AAS_AICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
    {
        if (AICharacter->GetCombatComponent())
        {
            if (AICharacter->GetCombatComponent()->GetEquippedWeapon())
            {
                AcceptableRadius = AICharacter->GetCombatComponent()->GetEquippedWeapon()->GetFireRange();
                return Super::ExecuteTask(OwnerComp, NodeMemory);
            }
        }
    }
    return EBTNodeResult::Failed;
}
