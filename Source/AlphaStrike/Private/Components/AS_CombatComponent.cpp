// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Components/AS_CombatComponent.h"
#include "Characters/AS_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapons/AS_BaseWeapon.h"
#include "Net/UnrealNetwork.h"

UAS_CombatComponent::UAS_CombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UAS_CombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UAS_CombatComponent, EquippedWeapon);
    DOREPLIFETIME(UAS_CombatComponent, bIsAiming);
}

EWeaponType UAS_CombatComponent::GetEquippedWeaponType() const
{
    if (EquippedWeapon)
    {
        return EquippedWeapon->GetWeaponType();
    }
    return EWeaponType::EWT_Empty;
}

void UAS_CombatComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!GetOwner()) return;

    PlayerCharacter = Cast<AAS_Character>(GetOwner());

    if (GetOwnerRole() == ENetRole::ROLE_Authority && GetWorld())
    {
        ACharacter* Owner = GetOwner<ACharacter>();
        if (!Owner) return;

        FActorSpawnParameters WeaponSpawnParameters;
        WeaponSpawnParameters.Instigator = Owner;
        EquippedWeapon = GetWorld()->SpawnActor<AAS_BaseWeapon>(EquippedWeaponClass, WeaponSpawnParameters);

        if (!Owner->GetMesh() || !EquippedWeapon) return;

        EquippedWeapon->SetOwner(Owner);
        FAttachmentTransformRules AttachmentTransformRules(
            EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
        EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules, EquippedWeapon->GetMuzzleSocketName());
    }
}

void UAS_CombatComponent::OnRep_EquippedWeapon()
{
    ACharacter* Owner = GetOwner<ACharacter>();
    if (!Owner || !Owner->GetMesh() || !EquippedWeapon) return;

    FAttachmentTransformRules AttachmentTransformRules(
        EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
    EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules, EquippedWeapon->GetMuzzleSocketName());
}

void UAS_CombatComponent::Aim()
{
    if (!PlayerCharacter) return;

    bIsAiming = true;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = MaxAimWalkSpeed;
    PlayerCharacter->GetFollowCamera()->FieldOfView = AimingFOV;

    if (PlayerCharacter->IsLocallyControlled())
    {
        Server_SetAim(bIsAiming);
    }
}

void UAS_CombatComponent::StopAim()
{
    if (!PlayerCharacter) return;

    bIsAiming = false;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = PlayerCharacter->GetDefaultWalkSpeed();
    PlayerCharacter->GetFollowCamera()->FieldOfView = PlayerCharacter->GetDefaultFOV();

    if (PlayerCharacter->IsLocallyControlled())
    {
        Server_SetAim(bIsAiming);
    }
}

void UAS_CombatComponent::Server_SetAim_Implementation(bool bAim)
{
    bIsAiming = bAim;
}

void UAS_CombatComponent::Fire()
{
    if (!EquippedWeapon) return;
    EquippedWeapon->Fire();
}

FVector UAS_CombatComponent::GetStartMuzzlePoint() const
{
    if (!EquippedWeapon) return FVector();
    return EquippedWeapon->GetStartMuzzlePoint();
}

FVector UAS_CombatComponent::GetEndMuzzlePoint() const
{
    if (!EquippedWeapon) return FVector();
    return EquippedWeapon->GetEndMuzzlePoint();
}
