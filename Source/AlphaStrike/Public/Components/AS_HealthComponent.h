// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeadDelegate, class AActor*, DeadActor, class AController*, InstigatedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, class AActor*, ChangedActor, bool, bDamage);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALPHASTRIKE_API UAS_HealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAS_HealthComponent();
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void OnUnregister();

public:
    FORCEINLINE float GetHealth() const { return Health; }
    FORCEINLINE float GetShield() const { return Shield; }
    FORCEINLINE bool IsFullHealth() const { return FMath::IsNearlyEqual(Health, MaxHealth); }
    FORCEINLINE bool IsFullShield() const { return FMath::IsNearlyEqual(Shield, MaxShield); }
    FORCEINLINE bool HasSomeShield() const { return !FMath::IsNearlyEqual(Shield, 0.f); }
    FORCEINLINE bool IsDead() const { return FMath::IsNearlyEqual(Health, 0.f); }
    float GetHealthPercent();
    float GetShieldPercent();
    FORCEINLINE bool IsInvincible() const { return bIsInvincible; }
    void SetInvincible(bool bNewValue, float InvincibilityTime);

    void SetHealth(const float NewHealth);
    void SubHealth(const float SubHealth);
    void AddHealth(const float AddHealth);
    void SetShield(const float NewShield);
    void AddShield(const float AddShield);

public:
    FOnDeadDelegate OnDeadDelegate;
    FOnHealthChangedDelegate OnHealthChangedDelegate;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "AS|Health")
    float MaxHealth = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health = 0.f;

    UPROPERTY(EditAnywhere, Category = "AS|Health")
    float MaxShield = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_Shield)
    float Shield = 0.f;

    UPROPERTY(EditAnywhere, Category = "AS")
    bool bEnableShiled = false;

    UPROPERTY(EditAnywhere, Category = "AS")
    bool bLogShow = false;

    UPROPERTY()
    AActor* OwnerCharacter;

    bool bIsInvincible = false;

    FTimerHandle FlickerHandle;
    FTimerHandle EndFlickerHandle;

private:
    /*
     * Multiplayer functions
     */
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_InvincibleFlicker(bool bNewValue, float InvincibilityTime);

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_Shield();

    void ServerSideBeginPlay();
    /*
     * Other functions
     */
    UFUNCTION()
    void OnTakeAnyDamageCallback(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void UpdateHealth(const float HealthToUpdate, bool bDamage);
    void CheckIsDead(AController* InstigatedBy);
    void LogShow();
    void VisibilityFlicker();
    void HandleInvincible(bool bNewValue, float InvincibilityTime);
};
