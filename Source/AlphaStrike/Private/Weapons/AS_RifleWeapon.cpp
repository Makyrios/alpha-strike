// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Weapons/AS_RifleWeapon.h"

void AAS_RifleWeapon::Fire()
{
    if (!GetWorld()) return;
    GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &AAS_RifleWeapon::Firing, AutomaticFireDelay, true, 0);
}

void AAS_RifleWeapon::StopFire()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);

    AAS_BaseWeapon::StopFire();
}

void AAS_RifleWeapon::Firing()
{
    AAS_BaseWeapon::Fire();
}
