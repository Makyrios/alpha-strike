// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Components/AS_BuffComponent.h"
#include "Components/AS_CombatComponent.h"
#include "Characters/AS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAS_BuffComponent::UAS_BuffComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAS_BuffComponent::ApplySpeedBuff(float BuffMultiplier, float BuffTime)
{
    if (!Character || !Character->GetCharacterMovement()) return;

    Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetDefaultWalkSpeed() * BuffMultiplier;
    Character->GetCharacterMovement()->MaxWalkSpeedCrouched = Character->GetDefaultCrouchWalkSpeed() * BuffMultiplier;

    Character->GetWorldTimerManager().SetTimer(  //
        SpeedBuffHandle,                         //
        this,                                    //
        &UAS_BuffComponent::ResetSpeedBuff,      //
        BuffTime                                 //
    );
}

void UAS_BuffComponent::ApplyJumpBuff(float BuffMultiplier, float BuffTime)
{
    if (!Character || !Character->GetCharacterMovement()) return;

    Character->GetCharacterMovement()->JumpZVelocity = Character->GetDefaultJumpZVelocity() * BuffMultiplier;

    Character->GetWorldTimerManager().SetTimer(  //
        JumpBuffHandle,                          //
        this,                                    //
        &UAS_BuffComponent::ResetJumpBuff,       //
        BuffTime                                 //
    );
}

void UAS_BuffComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!GetOwner()) return;
    Character = Cast<AAS_Character>(GetOwner());
}

void UAS_BuffComponent::ResetSpeedBuff()
{
    if (!Character || !Character->GetCharacterMovement() || !Character->GetCombatComponent()) return;

    const bool bCharacterAiming = Character->GetCombatComponent()->IsAiming();
    const float DiffWalkSpeed = Character->GetCombatComponent()->DiffAimWalkSpeed;
    const float CharacterSpeed = Character->GetDefaultWalkSpeed();

    Character->GetCharacterMovement()->MaxWalkSpeed = (bCharacterAiming) ? CharacterSpeed - DiffWalkSpeed : CharacterSpeed;
    Character->GetCharacterMovement()->MaxWalkSpeedCrouched = Character->GetDefaultCrouchWalkSpeed();
}

void UAS_BuffComponent::ResetJumpBuff()
{
    if (!Character || !Character->GetCharacterMovement()) return;

    Character->GetCharacterMovement()->JumpZVelocity = Character->GetDefaultJumpZVelocity();
}
