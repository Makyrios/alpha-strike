// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Tasks/AS_BTTaskNode_Fire.h"
#include "AI/AS_AIController.h"

EBTNodeResult::Type UBTTaskNode_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (!OwnerComp.GetAIOwner()) return EBTNodeResult::Failed;

    AAS_AIController* AIController = Cast<AAS_AIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AIController->Fire();
    return EBTNodeResult::Succeeded;
}
