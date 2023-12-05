// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AI/AS_AICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAS_AICharacter::AAS_AICharacter() 
{
    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}
