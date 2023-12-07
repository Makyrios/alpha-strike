// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Services/AS_BTService_FindEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Components/AS_AIPerceptionComponent.h"
#include "AIController.h"

UBTService_FindEnemy::UBTService_FindEnemy()
{
    NodeName = "Find Enemy";
}

void UBTService_FindEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        if (!Controller) return;

        const auto PerceptionComponent = Controller->GetComponentByClass<UAS_AIPerceptionComponent>();
        if (PerceptionComponent)
        {
            AActor* ClosestEnemy = PerceptionComponent->GetClosestEnemy();
            if (ClosestEnemy != nullptr)
            {
                Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, ClosestEnemy);
                Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, ClosestEnemy->GetActorLocation());
            }
            else
            {
                Blackboard->ClearValue(EnemyActorKey.SelectedKeyName);
                Blackboard->ClearValue(AimLocationKey.SelectedKeyName);
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
