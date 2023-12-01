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

    DOREPLIFETIME(UAS_CombatComponent, EquippedWeaponIndex);
    DOREPLIFETIME(UAS_CombatComponent, WeaponInventory);
    DOREPLIFETIME(UAS_CombatComponent, bIsAiming);
}

AAS_BaseWeapon* UAS_CombatComponent::GetEquippedWeapon() const
{
    if (WeaponInventory.IsValidIndex(EquippedWeaponIndex))
    {
        return WeaponInventory[EquippedWeaponIndex];
    }
    return nullptr;
}

EWeaponType UAS_CombatComponent::GetEquippedWeaponType() const
{
    if (WeaponInventory.IsValidIndex(EquippedWeaponIndex))
    {
        return WeaponInventory[EquippedWeaponIndex]->GetWeaponType();
    }
    return EWeaponType::EWT_Empty;
}

void UAS_CombatComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!GetOwner()) return;

    PlayerCharacter = Cast<AAS_Character>(GetOwner());

    SpawnWeapon();
}

void UAS_CombatComponent::SpawnWeapon()
{
    if (GetOwnerRole() == ENetRole::ROLE_Authority && GetWorld())
    {
        ACharacter* Owner = GetOwner<ACharacter>();
        if (!Owner) return;

        FActorSpawnParameters WeaponSpawnParameters;
        WeaponSpawnParameters.Instigator = Owner;
        AAS_BaseWeapon* EquippedWeapon = GetWorld()->SpawnActor<AAS_BaseWeapon>(EquippedWeaponClass, WeaponSpawnParameters);

        if (!Owner->GetMesh() || !EquippedWeapon) return;

        EquippedWeapon->SetOwner(Owner);
        FAttachmentTransformRules AttachmentTransformRules(
            EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
        EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules, EquippedWeapon->GetMuzzleSocketName());

        WeaponInventory.Add(EquippedWeapon);
        EquippedWeaponIndex = 0;
    }
}

void UAS_CombatComponent::OnRep_EquippedWeaponIndex()
{
    if (!WeaponInventory.IsValidIndex(PreviousWeaponIndex) || !WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return;
    AAS_BaseWeapon* PreviousWeapon = WeaponInventory[PreviousWeaponIndex];
    AAS_BaseWeapon* CurrentWeapon = WeaponInventory[EquippedWeaponIndex];
    PreviousWeapon->GetWeaponMesh()->SetVisibility(false);
    CurrentWeapon->GetWeaponMesh()->SetVisibility(true);

    if (!PlayerCharacter) return;
    PlayerCharacter->UpdateHUDAmmoInfo();
}

void UAS_CombatComponent::ChangeWeapon_Implementation(int WeaponIndex)
{
    if (WeaponInventory.Num() < 2) return;
    if (WeaponIndex < 0)
    {
        WeaponIndex = WeaponInventory.Num() - 1;
    }
    else if (WeaponIndex >= WeaponInventory.Num())
    {
        WeaponIndex = 0;
    }
    if (!WeaponInventory.IsValidIndex(WeaponIndex)) return;

    PreviousWeaponIndex = EquippedWeaponIndex;
    EquippedWeaponIndex = WeaponIndex;

    if (!WeaponInventory.IsValidIndex(PreviousWeaponIndex) || !WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return;
    AAS_BaseWeapon* PreviousWeapon = WeaponInventory[PreviousWeaponIndex];
    AAS_BaseWeapon* CurrentWeapon = WeaponInventory[EquippedWeaponIndex];
    PreviousWeapon->GetWeaponMesh()->SetVisibility(false);
    CurrentWeapon->GetWeaponMesh()->SetVisibility(true);
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
    if (!WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return;
    WeaponInventory[EquippedWeaponIndex]->Fire();
}

void UAS_CombatComponent::StopFire()
{
    if (!WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return;
    WeaponInventory[EquippedWeaponIndex]->StopFire();
}

void UAS_CombatComponent::ScrollWeaponUp()
{
    if (WeaponInventory.Num() > 1)
    {
        ChangeWeapon(EquippedWeaponIndex - 1);
    }
}

void UAS_CombatComponent::ScrollWeaponDown()
{
    if (WeaponInventory.Num() > 1)
    {
        ChangeWeapon(EquippedWeaponIndex + 1);
    }
}
FVector UAS_CombatComponent::GetStartMuzzlePoint() const
{
    if (!WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return FVector();
    return WeaponInventory[EquippedWeaponIndex]->GetStartMuzzlePoint();
}

FVector UAS_CombatComponent::GetEndMuzzlePoint() const
{
    if (!WeaponInventory.IsValidIndex(EquippedWeaponIndex)) return FVector();
    return WeaponInventory[EquippedWeaponIndex]->GetEndMuzzlePoint();
}

AAS_BaseWeapon* UAS_CombatComponent::FindWeaponOfClass(TSubclassOf<AAS_BaseWeapon> WeaponClass)
{
    AAS_BaseWeapon** FoundWeapon = WeaponInventory.FindByPredicate([&](AAS_BaseWeapon* W1) { return W1->GetClass() == WeaponClass.Get(); });
    return FoundWeapon ? *FoundWeapon : nullptr;
}

void UAS_CombatComponent::AddWeaponToInventory(AAS_BaseWeapon* NewWeapon)
{
    WeaponInventory.Add(NewWeapon);
}