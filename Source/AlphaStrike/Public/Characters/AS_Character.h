// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AS_Types.h"
#include "AS_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UAS_CombatComponent;
class UAS_HealthComponent;
class UAnimMontage;
class AAS_BaseWeapon;
class USplineComponent;
class USplineMeshComponent;
class AAS_PlayerController;

UCLASS()
class ALPHASTRIKE_API AAS_Character : public ACharacter
{
    GENERATED_BODY()

public:
    AAS_Character();
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void Tick(float DeltaTime) override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;

    void CrosshairActivate(const FVector& StartLocation, const FVector& EndLocation);
    void CrosshairDeactivate();

    void Die();

public:
    FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    FORCEINLINE UAS_CombatComponent* GetCombatComponent() const { return CombatComponent; }
    FORCEINLINE UAS_HealthComponent* GetHealthComponent() const { return HealthComponent; }
    FORCEINLINE const float GetDefaultWalkSpeed() const { return DefaultWalkSpeed; }
    FORCEINLINE const float GetDefaultCrouchWalkSpeed() const { return DefaultCrouchWalkSpeed; }
    FORCEINLINE const float GetDefaultFOV() const { return DefaultFOV; }
    FORCEINLINE ETurningInPlace GetTurningState() const { return TurningInPlace; }
    FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
    FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
    FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
    AAS_BaseWeapon* GetEquippedWeapon() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    UAS_HealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    UAS_CombatComponent* CombatComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Components")
    USplineComponent* CrosshairComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AS|Animations", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* DeathAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Crosshair")
    float CrosshairDepth = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Crosshair")
    UStaticMesh* SplineMesh;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Crosshair")
    UMaterial* SplineMaterial;

protected:
    virtual void BeginPlay() override;

    void DrawCrosshair(const FVector& StartLocation, const FVector& EndLocation);
    void ClearCrosshair();

private:
    UPROPERTY(EditDefaultsOnly, Category = "AS|Movement")
    float AngleToTurn = 80.f;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Movement")
    float DeathDestroyDelay = 5.f;

    UPROPERTY()
    USplineMeshComponent* SplineMeshComponent;

    UPROPERTY()
    AAS_PlayerController* PlayerController;

    float DefaultWalkSpeed = 0.f;
    float DefaultCrouchWalkSpeed = 0.f;
    float DefaultFOV = 0.f;

    UPROPERTY(Replicated)
    float AO_Yaw = 0.f;

    UPROPERTY(Replicated)
    float AO_Pitch = 0.f;

    float InterpAO_Yaw = 0.f;

    UPROPERTY(Replicated)
    ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

    FRotator StartAimRotation;
    float TimeSinceLastMovementReplication = 0.0f;
    bool bRotateRootBone = false;

private:
    UFUNCTION(Client, Reliable)
    void Client_DamageCallback(float HealthPercent, float ShieldPercent);

    UFUNCTION()
    void OnDamageCallback(AActor* DamagedActor);

    void RotateInPlace(float DeltaTime);
    void UpdateAimOffset(float DeltaTime);
    void SetTurningInPlace(float DeltaTime);
    void UpdateIfIsNotStanding();
    void CalculateAO_Pitch();

    float CalculateSpeed();
};
