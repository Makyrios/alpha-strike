// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/Decorators/AS_NeedHealthDecorator.h"
#include "AIController.h"
#include "Components/AS_HealthComponent.h"

UAS_NeedHealthDecorator::UAS_NeedHealthDecorator()
{
    NodeName = "Health Percent";
}

bool UAS_NeedHealthDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Controller->GetPawn()) return false;

    const auto HealthComponent = Controller->GetPawn()->GetComponentByClass<UAS_HealthComponent>();
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    return HealthComponent->GetHealthPercent() < MinPercentForHeal;
}
