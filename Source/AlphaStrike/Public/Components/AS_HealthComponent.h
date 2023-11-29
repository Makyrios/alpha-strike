// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AS_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDelegate, AActor*, DeadActor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALPHASTRIKE_API UAS_HealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAS_HealthComponent();
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
    FORCEINLINE float GetHealth() const { return Health; }
    FORCEINLINE bool IsFullHealth() const { return FMath::IsNearlyEqual(Health, MaxHealth); }
    FORCEINLINE bool IsDead() const { return FMath::IsNearlyEqual(Health, 0.f); }
    FORCEINLINE bool IsInvincible() const { return bIsInvincible; }
    void SetInvincible(bool bNewValue, float InvincibilityTime);

    void SetHealth(const float NewHealth);
    void SubHealth(const float SubHealth);

public:
    FOnDeadDelegate OnDeadDelegate;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "AS|Health")
    float MaxHealth = 100.f;

    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health = 0.f;

    UPROPERTY(EditAnywhere, Category = "AS|Health|Log")
    bool bLogShowHealth = false;

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
    void Multicast_OnDead();

    UFUNCTION()
    void OnRep_Health();

    void ServerSideBeginPlay();
    /*
     * Other functions
     */
    UFUNCTION()
    void OnTakeAnyDamageCallback(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void UpdateHealth(const float HealthToUpdate);
    void CheckIsDead(AController* InstigatedBy);
    void LogShowHealth();
    void SetHealthForHUD();
    float GetHealthPercent();
    void VisibilityFlicker();
};
