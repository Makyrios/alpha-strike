// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AnimInstances/AS_CharacterAnimInstance.h"
#include "Characters/AS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AS_CombatComponent.h"
#include "Weapons/AS_BaseWeapon.h"

void UAS_CharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Character = Cast<AAS_Character>(TryGetPawnOwner());
    if (Character)
    {
        Movement = Character->GetCharacterMovement();
    }
}

void UAS_CharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // Check character with movement
    if (!Character)
    {
        Character = Cast<AAS_Character>(TryGetPawnOwner());
        if (!Character) return;

        Movement = Character->GetCharacterMovement();
        if (!Movement) return;
    }

    // Set character speed
    FVector Velocity = Character->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    // Set direction
    {
        FRotator ActorRot = Character->GetActorRotation();
        FRotator VelocityDirection = UKismetMathLibrary::Conv_VectorToRotator(Character->GetVelocity());
        FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(ActorRot, VelocityDirection);
        Direction = DeltaRot.Yaw * -1;
    }

    // Set is the character in the air
    bIsInAir = Movement->IsFalling();

    // Set does the character accelerate
    bIsAccelerating = Movement->GetCurrentAcceleration().Size() > 0;

    // Set is the character is crouched
    bIsCrouched = Character->bIsCrouched;

    // Set yaw, roll and pitch
    const FRotator BaseAimRotation = Character->GetBaseAimRotation();
    const FRotator CurrentRotation = Character->GetActorRotation();
    const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(BaseAimRotation, CurrentRotation);
    Yaw = DeltaRot.Yaw;
    Roll = DeltaRot.Roll;
    Pitch = DeltaRot.Pitch;

    AO_Yaw = Character->GetAO_Yaw();
    AO_Pitch = Character->GetAO_Pitch();

    bIsFullBody = GetCurveValue(FName("FullBody")) > 0;

    bIsAiming = Character->GetCombatComponent()->IsAiming();

    EquippedWeapon = Character->GetEquippedWeapon();

    if (EquippedWeapon)
    {
        WeaponType = EquippedWeapon->GetWeaponType();
    }

    // Set if character need to turn right or left while standing
    TurningInPlace = Character->GetTurningState();

    // Sets whether or not to rotate the root bone (necessary if not a simulated proxy)
    bRotateRootBone = Character->ShouldRotateRootBone();

    // For FABRIK
    if (EquippedWeapon && EquippedWeapon->GetWeaponMesh() && Character->GetMesh())
    {
        // Set world transform of LeftHandSocket on equipped weapon (all weapons must be have this socket)
        LeftHandTransform =
            EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);

        FVector OutPosition;
        FRotator OutRotation;

        // Transform world location and rotation to bone (local) space and save it in OutPosition and OutRotation
        Character->GetMesh()->TransformToBoneSpace(
            FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);

        // Set completed variables to LeftHandTransform for using in blueprints
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat4d(OutRotation));
    }
}
