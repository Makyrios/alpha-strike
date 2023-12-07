// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Weapons/AS_BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AS_AmmoComponent.h"
#include "Characters/AS_Character.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"

AAS_BaseWeapon::AAS_BaseWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponMesh"));
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponMesh->SetIsReplicated(true);
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
    if (!AmmoComponent || !GetWorld()) return;

    if (AmmoComponent->CanShoot())
    {
        if (bCanFire)
        {
            bCanFire = false;
            bCanPlayNoAmmoSound = true;

            AmmoComponent->Server_HandleWeaponFired();
            Server_ApplyDamage(HitTarget.GetActor(), HitTarget);
            GetWorld()->GetTimerManager().SetTimer(
                FireDelayTimer, [this]() { bCanFire = true; }, FireDelay, false);
        }
    }
    else if (AmmoComponent->CanReload())
    {
        StartReload();
    }
    else if (NoAmmoSound && bCanPlayNoAmmoSound)
    {
        bCanPlayNoAmmoSound = false;

        UGameplayStatics::PlaySoundAtLocation(  //
            GetWorld(),                         //
            NoAmmoSound,                        //
            GetActorLocation()                  //
        );
    }
}

void AAS_BaseWeapon::StopFire() 
{
    bCanPlayNoAmmoSound = true;
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

void AAS_BaseWeapon::StartReload()
{
    if (!AmmoComponent || !AmmoComponent->CanReload()) return;

    Server_StartReload();
}

void AAS_BaseWeapon::FinishReload()
{
    if (AmmoComponent)
    {
        bReload = false;
        AmmoComponent->Server_Reload();
    }
}

void AAS_BaseWeapon::StartChangeWeapon()
{
    Server_StartChangeWeapon();
}

void AAS_BaseWeapon::FinishChangeWeapon() {}

void AAS_BaseWeapon::Multicast_Fire_Implementation(const FHitResult& HitResult)
{
    if (FireAnimation && WeaponMesh)
    {
        WeaponMesh->PlayAnimation(FireAnimation, false);
    }

    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(  //
            GetWorld(),                         //
            FireSound,                          //
            GetActorLocation()                  //
        );
    }

    if (HitResult.bBlockingHit)
    {
        SpawnImpactParticles(HitResult);
        SpawnHitDecals(HitResult);
    }

    SpawnBeamParticles(HitResult);
}

void AAS_BaseWeapon::Multicast_StartReload_Implementation()
{
    if (!ReloadAnimMontage || !AS_Owner || bReload) return;

    if (AmmoComponent)
    {
        FAmmoInfo AmmoInfo = AmmoComponent->GetAmmoInfo();
        if (AmmoInfo.CurrentAmmo < AmmoInfo.MaxAmmoInClip)
        {
            bReload = true;
            AS_Owner->PlayAnimMontage(ReloadAnimMontage);
        }
    }
}

void AAS_BaseWeapon::Multicast_StartChangeWeapon_Implementation()
{
    if (!ChangeWeaponAnimMontage || !AS_Owner) return;

    AS_Owner->PlayAnimMontage(ChangeWeaponAnimMontage);
}

void AAS_BaseWeapon::SpawnImpactParticles(const FHitResult& HitResult)
{
    if (ImpactParticles && ImpactSound)
    {
        UGameplayStatics::SpawnEmitterAtLocation(  //
            GetWorld(),                            //
            ImpactParticles,                       //
            HitResult.ImpactPoint,                 //
            HitResult.ImpactNormal.Rotation()      //
        );

        UGameplayStatics::PlaySoundAtLocation(  //
            GetWorld(),                         //
            ImpactSound,                        //
            HitResult.ImpactPoint               //
        );
    }
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

void AAS_BaseWeapon::SpawnHitDecals(const FHitResult& HitResult)
{
    if (HitDecalMaterial)
    {
        const FRotator DecalRotation = UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal);

        auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(
            this, HitDecalMaterial, HitDecalSize, HitResult.ImpactPoint, DecalRotation, HitDecalLifeSpan);

        if (DecalComponent)
        {
            DecalComponent->SetFadeOut(HitDecalLifeSpan, HitDecalFadeOutTime);
        }
    }
}

void AAS_BaseWeapon::Server_StartReload_Implementation()
{
    Multicast_StartReload();
}

void AAS_BaseWeapon::Server_StartChangeWeapon_Implementation()
{
    Multicast_StartChangeWeapon();
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

    DrawDebugSphere(GetWorld(), End, 6.f, 6, FColor::Red, false, -1.f, 0, 2.f);
}

void AAS_BaseWeapon::SetOwner(AActor* NewOwner)
{
    Super::SetOwner(NewOwner);

    HandleAmmoChange();
}

FText AAS_BaseWeapon::GetAmmoInfoText()
{
    if (!AmmoComponent) return FText();

    return AmmoComponent->GetAmmoInfoAsText();
}

void AAS_BaseWeapon::HandleAmmoChange()
{
    if (!AmmoComponent) return;

    AS_Owner = (!AS_Owner) ? GetOwner<AAS_Character>() : AS_Owner;
    if (!AS_Owner) return;

    AS_Owner->UpdateHUDAmmoInfo();
}

void AAS_BaseWeapon::Server_ToggleVisibility_Implementation()
{
    if (WeaponMesh)
    {
        WeaponMesh->ToggleVisibility();
    }
}
