// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Tasks/AS_BTTask_AimAtTarget.h"
#include "AI/AS_AIController.h"

EBTNodeResult::Type UBTTask_AimAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (!OwnerComp.GetAIOwner() || !Blackboard) return EBTNodeResult::Failed;

    AAS_AIController* AIController = Cast<AAS_AIController>(OwnerComp.GetAIOwner());
    if (!AIController) return EBTNodeResult::Failed;

    AIController->Aim();
    return EBTNodeResult::Succeeded;
}
