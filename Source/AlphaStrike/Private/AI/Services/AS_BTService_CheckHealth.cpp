// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Services/AS_BTService_CheckHealth.h"
#include "AI/AS_AICharacter.h"
#include "Components/AS_HealthComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard || !OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn()) return;

    if (AAS_AICharacter* AICharacter = Cast<AAS_AICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
    {
        if (AICharacter->GetHealthComponent())
        {
            Blackboard->SetValueAsFloat(BlackboardKey.SelectedKeyName, AICharacter->GetHealthComponent()->GetHealth());
        }
    }

}
