// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Weapons/AS_BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AS_AmmoComponent.h"

AAS_BaseWeapon::AAS_BaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetRootComponent(WeaponMesh);

    AmmoComponent = CreateDefaultSubobject<UAS_AmmoComponent>(FName("AmmoComponent"));

    bReplicates = true;
}

void AAS_BaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateHitTarget();
}

void AAS_BaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void AAS_BaseWeapon::Fire()
{
    if (AmmoComponent->CanShoot())
    {
        AmmoComponent->HandleWeaponFired();
        Server_ApplyDamage(HitTarget.GetActor(), HitTarget);
    }
    else
    {
        Reload();
    }
}

void AAS_BaseWeapon::Server_ApplyDamage_Implementation(AActor* DamagedActor, const FHitResult& HitResult)
{
    if (GetOwner() && GetInstigatorController() && DamagedActor)
    {
        if (GetOwner() != DamagedActor)
        {
            // Do damage only on the server side
            UGameplayStatics::ApplyDamage(  //
                DamagedActor,               //
                Damage,                     //
                GetInstigatorController(),  //
                this,                       //
                UDamageType::StaticClass()  //
            );
        }
    }

    Multicast_Fire(HitResult);
}

void AAS_BaseWeapon::Reload()
{
    AmmoComponent->Reload();
}

void AAS_BaseWeapon::Multicast_Fire_Implementation(const FHitResult& HitResult)
{
    if (FireAnimation && WeaponMesh)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    if (HitResult.bBlockingHit)
    {
        if (ImpactParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(  //
                GetWorld(),                            //
                ImpactParticles,                       //
                HitResult.ImpactPoint,                 //
                HitResult.ImpactNormal.Rotation()      //
            );
        }
    }

    SpawnBeamParticles(HitResult);
}

void AAS_BaseWeapon::UpdateHitTarget()
{
    if (!GetWorld() || !WeaponMesh) return;

    const FTransform MuzzleSocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
    FVector MuzzleForwardVector = MuzzleSocketTransform.GetRotation().Vector();
    MuzzleForwardVector.Normalize();

    HitStart = MuzzleSocketTransform.GetLocation();
    HitEnd = HitStart + MuzzleForwardVector * TraceLength;

    DoFireHit();
}

void AAS_BaseWeapon::DoFireHit()
{
    if (!GetWorld() || !GetOwner()) return;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    GetWorld()->LineTraceSingleByChannel(   //
        HitTarget,                          //
        HitStart,                           //
        HitEnd,                             //
        ECollisionChannel::ECC_Visibility,  //
        Params                              //
    );
}

void AAS_BaseWeapon::DrawDebugFireTrace(const FVector& Start, const FVector& End)
{
    if (!GetWorld()) return;

    /* DrawDebugLine(  //
         GetWorld(),   //
         Start,        //
         End,          //
         FColor::Red,  //
         false,        //
         2.f,          //
         0,            //
         2.f           //
     );*/

    DrawDebugSphere(GetWorld(), End, 6.f, 6, FColor::Red, false, -1.f, 0, 2.f);
}

void AAS_BaseWeapon::SpawnBeamParticles(const FHitResult& HitResult)
{
    FVector BeamEnd = HitEnd;
    if (HitResult.bBlockingHit)
    {
        BeamEnd = HitResult.ImpactPoint;
    }

    if (BeamParticles)
    {
        UParticleSystemComponent* BeamSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(  //
            GetWorld(),                                                                            //
            BeamParticles,                                                                         //
            HitStart,                                                                              //
            FRotator::ZeroRotator,                                                                 //
            true                                                                                   //
        );

        if (BeamSystemComponent)
        {
            BeamSystemComponent->SetVectorParameter(FName("Target"), BeamEnd);
        }
    }
}

FText AAS_BaseWeapon::GetAmmoInfoAsText()
{
    return AmmoComponent->GetAmmoInfoAsText();
}