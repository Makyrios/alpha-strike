// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Components/AS_AmmoComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/AS_BaseWeapon.h"

UAS_AmmoComponent::UAS_AmmoComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    SetIsReplicatedByDefault(true);
}

void UAS_AmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UAS_AmmoComponent, AmmoInfo);
}

void UAS_AmmoComponent::BeginPlay()
{
    Super::BeginPlay();

    if (GetOwnerRole() == ENetRole::ROLE_Authority)
    {
        OwnerWeapon = (!OwnerWeapon) ? GetOwner<AAS_BaseWeapon>() : OwnerWeapon;
        if (!OwnerWeapon || !AmmoInfo.bFiniteAmmo) return;

        AmmoInfo.CurrentAmmo = AmmoInfo.MaxAmmoInClip;
        AmmoInfo.AmmoInBag -= AmmoInfo.MaxAmmoInClip;
        OwnerWeapon->HandleAmmoChange();
    }
}

void UAS_AmmoComponent::Server_HandleWeaponFired_Implementation()
{
    --AmmoInfo.CurrentAmmo;
    if (AmmoInfo.CurrentAmmo == 0)
    {
        Server_Reload_Implementation();
    }

    OwnerWeapon = (!OwnerWeapon) ? GetOwner<AAS_BaseWeapon>() : OwnerWeapon;
    if (!OwnerWeapon) return;

    OwnerWeapon->HandleAmmoChange();
}

bool UAS_AmmoComponent::CanShoot() const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    bool bIsReloading = World->GetTimerManager().TimerExists(ReloadTimer) && World->GetTimerManager().IsTimerActive(ReloadTimer);

    return (!(AmmoInfo.bFiniteAmmo) || AmmoInfo.CurrentAmmo != 0) && !bIsReloading;
}

bool UAS_AmmoComponent::CanReload() const
{
    return AmmoInfo.AmmoInBag != 0;
}

void UAS_AmmoComponent::AddAmmoInBag(int AmmoAmount)
{
    OwnerWeapon = (!OwnerWeapon) ? GetOwner<AAS_BaseWeapon>() : OwnerWeapon;
    if (!OwnerWeapon) return;

    AmmoInfo.AmmoInBag += AmmoAmount;
    OwnerWeapon->HandleAmmoChange();
}

void UAS_AmmoComponent::Server_Reload_Implementation()
{
    UWorld* World = GetWorld();
    if (!CanReload() && World) return;

    if (!(World->GetTimerManager().IsTimerActive(ReloadTimer)))
    {
        World->GetTimerManager().SetTimer(ReloadTimer, this, &UAS_AmmoComponent::ReloadAmmo, ReloadTime, false);
    }
}

void UAS_AmmoComponent::ReloadAmmo()
{
    OwnerWeapon = (!OwnerWeapon) ? GetOwner<AAS_BaseWeapon>() : OwnerWeapon;
    if (!OwnerWeapon) return;

    int32 AmmoToAdd;
    if ((AmmoInfo.MaxAmmoInClip - AmmoInfo.CurrentAmmo) > AmmoInfo.AmmoInBag)
    {
        AmmoToAdd = AmmoInfo.AmmoInBag;
    }
    else
    {
        AmmoToAdd = AmmoInfo.MaxAmmoInClip - AmmoInfo.CurrentAmmo;
    }

    AmmoInfo.AmmoInBag -= AmmoToAdd;
    AmmoInfo.CurrentAmmo += AmmoToAdd;

    OwnerWeapon->HandleAmmoChange();
}

FText UAS_AmmoComponent::GetAmmoInfoAsText()
{
    FString AmmoString = FString::FromInt(AmmoInfo.CurrentAmmo) + " / " + FString::FromInt(AmmoInfo.AmmoInBag);
    return FText::FromString(AmmoString);
}

void UAS_AmmoComponent::OnRep_AmmoInfo()
{
    OwnerWeapon = (!OwnerWeapon) ? GetOwner<AAS_BaseWeapon>() : OwnerWeapon;
    if (!OwnerWeapon) return;

    OwnerWeapon->HandleAmmoChange();
}
