// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Components/AS_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include <AI/AS_AICharacter.h>
#include "UI/HUD/AS_HUD.h"
#include "Weapons/AS_BaseWeapon.h"

DEFINE_LOG_CATEGORY_STATIC(AS_HealthComponentLog, All, All);

UAS_HealthComponent::UAS_HealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UAS_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UAS_HealthComponent, Health);
    DOREPLIFETIME(UAS_HealthComponent, Shield);
}

void UAS_HealthComponent::OnUnregister() 
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(FlickerHandle);
        GetWorld()->GetTimerManager().ClearTimer(EndFlickerHandle);
    }

    Super::OnUnregister();
}

void UAS_HealthComponent::BeginPlay()
{
    Super::BeginPlay();

    ServerSideBeginPlay();
}

void UAS_HealthComponent::ServerSideBeginPlay()
{
    if (!GetOwner() || GetOwnerRole() != ENetRole::ROLE_Authority) return;

    // Update health and set damage callback only on the server side
    SetHealth(MaxHealth);
    SetShield(MaxShield);

    OnDamageDelegate.Broadcast(GetOwner());
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAS_HealthComponent::OnTakeAnyDamageCallback);
}

void UAS_HealthComponent::OnTakeAnyDamageCallback(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (IsDead() || bIsInvincible || !GetOwner()) return;

    SubHealth(Damage);
    CheckIsDead(InstigatedBy);
}

void UAS_HealthComponent::SetInvincible(bool bNewValue, float InvincibilityTime)
{
    bIsInvincible = bNewValue;

    HandleInvincible(bNewValue, InvincibilityTime);
    Multicast_InvincibleFlicker(bNewValue, InvincibilityTime);
}

void UAS_HealthComponent::Multicast_InvincibleFlicker_Implementation(bool bNewValue, float InvincibilityTime)
{
    HandleInvincible(bNewValue, InvincibilityTime);
}

void UAS_HealthComponent::HandleInvincible(bool bNewValue, float InvincibilityTime)
{
    if (!GetWorld()) return;

    if (bNewValue)
    {
        GetWorld()->GetTimerManager().SetTimer(FlickerHandle, this, &UAS_HealthComponent::VisibilityFlicker, 0.1, true);
        GetWorld()->GetTimerManager().SetTimer(
            EndFlickerHandle, [&]() { SetInvincible(false, 0); }, InvincibilityTime, false);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(FlickerHandle);
        GetWorld()->GetTimerManager().ClearTimer(EndFlickerHandle);

        AAS_Character* OwnerChar = GetOwner<AAS_Character>();
        if (OwnerChar && OwnerChar->GetMesh() && OwnerChar->GetEquippedWeapon() && OwnerChar->GetEquippedWeapon()->GetWeaponMesh())
        {
            OwnerChar->GetMesh()->SetVisibility(true);
            OwnerChar->GetEquippedWeapon()->GetWeaponMesh()->SetVisibility(true);
        }
    }
}

void UAS_HealthComponent::SetHealth(const float NewHealth)
{
    UpdateHealth(NewHealth);
}

void UAS_HealthComponent::SubHealth(const float SubHealth)
{
    if (bEnableShiled && HasSomeShield())
    {
        if (Shield > SubHealth)
        {
            Shield -= SubHealth;
            LogShow();
        }
        else
        {
            const float Damage = SubHealth - Shield;
            Shield = 0.f;
            UpdateHealth(Health - Damage);
        }
        OnDamageDelegate.Broadcast(GetOwner());
    }
    else
    {
        UpdateHealth(Health - SubHealth);
    }
}

void UAS_HealthComponent::AddHealth(const float AddHealth)
{
    SetHealth(Health + AddHealth);
}

void UAS_HealthComponent::SetShield(const float NewShield)
{
    Shield = FMath::Clamp(NewShield, 0.f, MaxShield);
    LogShow();
}

void UAS_HealthComponent::AddShield(const float AddShield)
{
    SetShield(Shield + AddShield);
    OnDamageDelegate.Broadcast(GetOwner());
}

void UAS_HealthComponent::UpdateHealth(const float HealthToUpdate)
{
    Health = FMath::Clamp(HealthToUpdate, 0.f, MaxHealth);
    OnDamageDelegate.Broadcast(GetOwner());

    LogShow();
}

void UAS_HealthComponent::OnRep_Health()
{
    LogShow();
}

void UAS_HealthComponent::OnRep_Shield()
{
    LogShow();
}

void UAS_HealthComponent::CheckIsDead(AController* InstigatedBy)
{
    if (IsDead() && GetOwner())
    {
        OnDeadDelegate.Broadcast(GetOwner(), InstigatedBy);
    }
}

void UAS_HealthComponent::LogShow()
{
    if (!GetOwner() || !bLogShow) return;

    UE_LOG(AS_HealthComponentLog, Display, TEXT("%s shiled: %f, health: %f"), *GetOwner()->GetName(), Shield, Health);
}

float UAS_HealthComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}

float UAS_HealthComponent::GetShieldPercent()
{
    return Shield / MaxShield;
}

void UAS_HealthComponent::VisibilityFlicker()
{
    if (AAS_Character* OwnerChar = Cast<AAS_Character>(GetOwner()))
    {
        OwnerChar->GetMesh()->ToggleVisibility();
        OwnerChar->GetEquippedWeapon()->GetWeaponMesh()->ToggleVisibility();
    }
}
