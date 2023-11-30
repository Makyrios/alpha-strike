// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AS_Types.h"
#include "AS_BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UAnimSequence;
class UParticleSystem;
class UAS_AmmoComponent;

UCLASS()
class ALPHASTRIKE_API AAS_BaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    AAS_BaseWeapon();

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    virtual void Fire();
    virtual void Reload();

    FText GetAmmoInfoAsText();

public:
    FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }
    FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
    FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
    FORCEINLINE FVector GetStartMuzzlePoint() const { return HitStart; }
    FORCEINLINE FVector GetEndMuzzlePoint() const { return HitTarget.bBlockingHit ? HitTarget.ImpactPoint : HitEnd; }

protected:
    UPROPERTY(VisibleAnywhere, Category = "AS|Components")
    USkeletalMeshComponent* WeaponMesh;

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

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UAnimSequence* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UParticleSystem* ImpactParticles;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UParticleSystem* BeamParticles;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Weapon properties")
    UAS_AmmoComponent* AmmoComponent;

    FHitResult HitTarget;
    FVector HitStart;
    FVector HitEnd;

private:
    /*
     * Multiplayer functions
     */
    UFUNCTION(Server, Reliable)
    void Server_ApplyDamage(AActor* DamagedActor, const FHitResult& HitResult);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Fire(const FHitResult& HitResult);

    /*
     * Other functions
     */
    void UpdateHitTarget();
    void DoFireHit();
    void DrawDebugFireTrace(const FVector& Start, const FVector& End);
    void SpawnBeamParticles(const FHitResult& HitResult);
};
