// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AS_Types.h"
#include "AS_BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UAnimSequence;
class UParticleSystem;
class USoundCue;
class UAS_AmmoComponent;
class AAS_Character;
class UAnimMontage;

UCLASS()
class ALPHASTRIKE_API AAS_BaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AAS_BaseWeapon();

    virtual void Tick(float DeltaTime) override;
    virtual void SetOwner(AActor* NewOwner) override;

    UFUNCTION(BlueprintCallable)
    virtual void Fire();
    virtual void StopFire();
    void StartReload();
    void FinishReload();
    void StartChangeWeapon();
    void FinishChangeWeapon();

    void HandleAmmoChange();

    UFUNCTION(Server, Reliable)
    void Server_ToggleVisibility();

public:
    FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }
    FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
    FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
    FORCEINLINE FVector GetStartMuzzlePoint() const { return HitStart; }
    FORCEINLINE FVector GetEndMuzzlePoint() const { return HitTarget.bBlockingHit ? HitTarget.ImpactPoint : HitEnd; }
    FORCEINLINE UAS_AmmoComponent* GetAmmoComponent() const { return AmmoComponent; }
    FORCEINLINE float GetFireRange() const { return TraceLength; }
    FORCEINLINE bool CanFire() const { return bCanFire; }
    FORCEINLINE void SetCanFire(bool NewValue) { bCanFire = NewValue; }
    FORCEINLINE UAnimMontage* GetReloadAnimation() const { return ReloadAnimMontage; }
    FORCEINLINE UAnimMontage* GetChangeWeaponAnimation() const { return ChangeWeaponAnimMontage; }
    FText GetAmmoInfoText();

protected:
    UPROPERTY(VisibleAnywhere, Category = "AS|Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Components")
    UAS_AmmoComponent* AmmoComponent;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Components")
    FName MuzzleSocketName = FName("MuzzleSocket");

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    EWeaponType WeaponType;

    UPROPERTY(EditAnywhere, Category = "AS|Weapon properties")
    float Damage = 10.f;

    UPROPERTY(EditAnywhere, Category = "AS|Weapon properties")
    float TraceLength = 10000.f;

    UPROPERTY(EditAnywhere, Category = "AS|Weapon properties")
    float FireDelay = 0.1f;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UAnimSequence* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UAnimMontage* ReloadAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UAnimMontage* ChangeWeaponAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UParticleSystem* ImpactParticles;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UParticleSystem* BeamParticles;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    USoundCue* ImpactSound;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    USoundCue* NoAmmoSound;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties|Decals")
    UMaterial* HitDecalMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties|Decals")
    FVector HitDecalSize = FVector(10.f, 10.f, 10.f);

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties|Decals")
    float HitDecalLifeSpan = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties|Decals")
    float HitDecalFadeOutTime = 0.7f;

    UPROPERTY()
    AAS_Character* AS_Owner;

    FHitResult HitTarget;
    FVector HitStart;
    FVector HitEnd;

    bool bCanFire = true;
    FTimerHandle FireDelayTimer;

private:
    /*
     * Multiplayer functions
     */
    UFUNCTION(Server, Reliable)
    void Server_ApplyDamage(AActor* DamagedActor, const FHitResult& HitResult);

    UFUNCTION(Server, Reliable)
    void Server_StartReload();

    UFUNCTION(Server, Reliable)
    void Server_StartChangeWeapon();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Fire(const FHitResult& HitResult);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartReload();

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_StartChangeWeapon();

    /*
     * Other functions
     */
    void UpdateHitTarget();
    void DoFireHit();
    void DrawDebugFireTrace(const FVector& Start, const FVector& End);
    void SpawnBeamParticles(const FHitResult& HitResult);
    void SpawnHitDecals(const FHitResult& HitResult);
    void SpawnImpactParticles(const FHitResult& HitResult);
};
