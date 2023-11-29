// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Components/AS_HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameModes/AS_BaseGameMode.h"
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
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAS_HealthComponent::OnTakeAnyDamageCallback);
}

void UAS_HealthComponent::OnTakeAnyDamageCallback(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (IsDead() || bIsInvincible) return;
    SubHealth(Damage);
    CheckIsDead(InstigatedBy);
}

void UAS_HealthComponent::SetInvincible(bool bNewValue, float InvincibilityTime)
{
    bIsInvincible = bNewValue;

    if (!GetWorld()) return;

    if (bIsInvincible)
    {
        GetWorld()->GetTimerManager().SetTimer(FlickerHandle, this, &UAS_HealthComponent::VisibilityFlicker, 0.1, true);
        GetWorld()->GetTimerManager().SetTimer(
            EndFlickerHandle, [&]() { SetInvincible(false, 0); }, InvincibilityTime, false);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(FlickerHandle);
        GetWorld()->GetTimerManager().ClearTimer(EndFlickerHandle);
        if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
        {
            OwnerChar->GetMesh()->SetVisibility(true);
        }
    }
}


void UAS_HealthComponent::SetHealth(const float NewHealth)
{
    UpdateHealth(NewHealth);
}

void UAS_HealthComponent::SubHealth(const float SubHealth)
{
    UpdateHealth(Health - SubHealth);
}

void UAS_HealthComponent::UpdateHealth(const float HealthToUpdate)
{
    Health = FMath::Clamp(HealthToUpdate, 0.f, MaxHealth);
    SetHealthForHUD();
    LogShowHealth();
}

void UAS_HealthComponent::SetHealthForHUD()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
    {
        if (AAS_HUD* PlayerHUD = PlayerController->GetHUD<AAS_HUD>())
        {
            PlayerHUD->SetHealthBarPercent(GetHealthPercent());
        }
    }
}

void UAS_HealthComponent::OnRep_Health()
{
    LogShowHealth();
}


void UAS_HealthComponent::CheckIsDead(AController* InstigatedBy)
{
    if (IsDead() && GetOwner())
    {
        // TODO replace to Controller class
        ACharacter* Owner = GetOwner<ACharacter>();
        if (!GetWorld() || !Owner) return;
        auto GameMode = GetWorld()->GetAuthGameMode<AAS_BaseGameMode>();
        if (GameMode)
        {
            GameMode->HandleActorDeath(Owner->GetController(), InstigatedBy);
        }

        OnDeadDelegate.Broadcast(GetOwner());

        Multicast_OnDead();
    }
}

void UAS_HealthComponent::Multicast_OnDead_Implementation()
{
    AAS_Character* Character = Cast<AAS_Character>(GetOwner());
    if (Character)
    {
        Character->Die();
    }
}

void UAS_HealthComponent::LogShowHealth()
{
    if (!GetOwner() || !bLogShowHealth) return;

    UE_LOG(AS_HealthComponentLog, Display, TEXT("%s health: %f"), *GetOwner()->GetName(), Health);
}

float UAS_HealthComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}

void UAS_HealthComponent::VisibilityFlicker()
{
    if (AAS_Character* OwnerChar = Cast<AAS_Character>(GetOwner()))
    {
        OwnerChar->GetMesh()->ToggleVisibility();
        OwnerChar->GetEquippedWeapon()->GetWeaponMesh()->ToggleVisibility();
    }
}
