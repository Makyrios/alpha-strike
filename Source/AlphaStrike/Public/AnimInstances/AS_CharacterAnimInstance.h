// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AS_Types.h"
#include "AS_CharacterAnimInstance.generated.h"

class AAS_Character;
class AAS_BaseWeapon;
class UCharacterMovementComponent;

UCLASS()
class ALPHASTRIKE_API UAS_CharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY(BlueprintReadOnly, Category = "AS|Character", meta = (AllowPrivateAccess = true))
    AAS_Character* Character;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Character", meta = (AllowPrivateAccess = true))
    bool bIsLocallyControlled = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Character", meta = (AllowPrivateAccess = true))
    EWeaponType WeaponType = EWeaponType::EWT_Empty;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float Speed = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float Direction = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float Yaw = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float Roll = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float Pitch = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    bool bIsInAir = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    bool bIsAccelerating = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    bool bIsFullBody = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    bool bIsCrouched = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float AO_Yaw = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Movement", meta = (AllowPrivateAccess = true))
    float AO_Pitch = 0.f;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Aiming", meta = (AllowPrivateAccess = true))
    bool bIsAiming = false;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Aiming", meta = (AllowPrivateAccess = true))
    bool bUseAimOffsets = true;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Aiming", meta = (AllowPrivateAccess = true))
    bool bRotateRootBone = true;

    UPROPERTY(BlueprintReadOnly, Category = "AS|Aiming", meta = (AllowPrivateAccess = true))
    FRotator RightHandRotation;

    UPROPERTY(BlueprintReadOnly, Category = "AS|FABRIK", meta = (AllowPrivateAccess = true))
    FTransform LeftHandTransform;

    UPROPERTY(BlueprintReadOnly, Category = "AS|FABRIK", meta = (AllowPrivateAccess = true))
    bool bUseFABRIK = true;

    FRotator CharacterRotationLastFrame;
    FRotator CharacterRotation;
    FRotator DeltaRotation;

    UPROPERTY()
    UCharacterMovementComponent* Movement;

    UPROPERTY()
    AAS_BaseWeapon* EquippedWeapon;
};
