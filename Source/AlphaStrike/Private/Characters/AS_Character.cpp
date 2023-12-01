// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Characters/AS_Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AS_HealthComponent.h"
#include "Components/AS_CombatComponent.h"
#include "Components/AS_BuffComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AS_AmmoComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/AS_BaseWeapon.h"
#include "Net/UnrealNetwork.h"
#include "Controllers/AS_PlayerController.h"

AAS_Character::AAS_Character()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UAS_HealthComponent>(TEXT("HealthComponent"));
    CombatComponent = CreateDefaultSubobject<UAS_CombatComponent>(TEXT("CombatComponent"));
    BuffComponent = CreateDefaultSubobject<UAS_BuffComponent>(TEXT("BuffComponent"));

    CrosshairComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Crosshair"));
    CrosshairComponent->SetupAttachment(GetRootComponent());
}

void AAS_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAS_Character, AO_Yaw);
    DOREPLIFETIME(AAS_Character, AO_Pitch);
    DOREPLIFETIME(AAS_Character, TurningInPlace);
}

void AAS_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    RotateInPlace(DeltaTime);
}

void AAS_Character::BeginPlay()
{
    Super::BeginPlay();

    if (HealthComponent)
    {
        HealthComponent->OnDamageDelegate.AddDynamic(this, &AAS_Character::OnDamageCallback);
    }

    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    DefaultCrouchWalkSpeed = GetCharacterMovement()->MaxWalkSpeedCrouched;
    DefaultJumpZVelocity = GetCharacterMovement()->JumpZVelocity;
    DefaultFOV = FollowCamera->FieldOfView;
}

void AAS_Character::PossessedBy(AController* NewController)
{
    if (!NewController) return;

    Super::PossessedBy(NewController);

    OnDamageCallback(this);
    UpdateHUDAmmoInfo();
}

void AAS_Character::UnPossessed()
{
    if (CombatComponent && CombatComponent->GetEquippedWeapon())
    {
        CombatComponent->GetEquippedWeapon()->Destroy();
    }

    Super::UnPossessed();
}

void AAS_Character::OnDamageCallback(AActor* DamagedActor)
{
    if (!HealthComponent || !HasAuthority()) return;

    if (IsLocallyControlled())
    {
        PlayerController = (!PlayerController) ? GetController<AAS_PlayerController>() : PlayerController;
        if (!PlayerController) return;

        PlayerController->SetHealthBarPercent(HealthComponent->GetHealthPercent());
        PlayerController->SetShieldBarPercent(HealthComponent->GetShieldPercent());
    }
    else
    {
        Client_DamageCallback(HealthComponent->GetHealthPercent(), HealthComponent->GetShieldPercent());
    }
}

void AAS_Character::Client_DamageCallback_Implementation(float HealthPercent, float ShieldPercent)
{
    if (!HealthComponent) return;

    PlayerController = (!PlayerController) ? GetController<AAS_PlayerController>() : PlayerController;
    if (!PlayerController) return;

    PlayerController->SetHealthBarPercent(HealthPercent);
    PlayerController->SetShieldBarPercent(ShieldPercent);
}

void AAS_Character::RotateInPlace(float DeltaTime)
{
    if (GetLocalRole() > ENetRole::ROLE_SimulatedProxy && IsLocallyControlled())
    {
        UpdateAimOffset(DeltaTime);
    }
    else
    {
        CalculateAO_Pitch();

        TimeSinceLastMovementReplication += DeltaTime;
        if (TimeSinceLastMovementReplication > 0.25f)
        {
            OnRep_ReplicateMovement();
        }
    }
}

void AAS_Character::UpdateAimOffset(float DeltaTime)
{
    CalculateAO_Pitch();

    const float Speed = CalculateSpeed();
    const bool bIsInAir = GetMovementComponent()->IsFalling();

    // Character is moving and we want to 'use control yaw rotation'
    // We need to save every frame StartAimRotation, and when we will stop, we can use it for calcucation delta rotation
    // And also we want to set yaw to 0.f for currect start using aim offset without glitching
    if (Speed > 0.f || bIsInAir)
    {
        bRotateRootBone = false;

        UpdateIfIsNotStanding();

        bUseControllerRotationYaw = true;
        return;
    }

    // Character stands and does not jump
    // So we can update yaw and pitch for aiming offsets
    // We must turn off 'use control yaw rotation', cause we want that character stay an same pose (legs) and move only hands
    if (FMath::IsNearlyZero(Speed) && !bIsInAir)
    {
        bRotateRootBone = true;

        const FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
        const FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartAimRotation);
        AO_Yaw = DeltaAimRotation.Yaw;

        if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
        {
            InterpAO_Yaw = AO_Yaw;
        }

        SetTurningInPlace(DeltaTime);

        bUseControllerRotationYaw = true;
    }
}

void AAS_Character::SetTurningInPlace(float DeltaTime)
{
    if (AO_Yaw > AngleToTurn)
    {
        TurningInPlace = ETurningInPlace::ETIP_Right;
    }
    else if (AO_Yaw < -AngleToTurn)
    {
        TurningInPlace = ETurningInPlace::ETIP_Left;
    }

    if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
    {
        InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
        AO_Yaw = InterpAO_Yaw;
        if (FMath::Abs(AO_Yaw) < 15.f)
        {
            StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
            TurningInPlace = ETurningInPlace::ETIP_NotTurning;
        }
    }
}

void AAS_Character::UpdateIfIsNotStanding()
{
    StartAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
    AO_Yaw = 0.f;
    TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void AAS_Character::CalculateAO_Pitch()
{
    // We can update pitch every frame
    AO_Pitch = GetBaseAimRotation().Pitch;

    // Fix for sending pitch to the simulated proxy if it greater then 90 degrees
    // Because when we sent it = negative float convert to signed number in [270; 360) area
    // We must convert it back to [-90; 0)
    if (AO_Pitch > 90.f && !IsLocallyControlled())
    {
        const FVector2D InRange(270.f, 360.f);
        const FVector2D OutRange(-90.f, 0.f);
        AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
    }
}

float AAS_Character::CalculateSpeed()
{
    FVector Velocity = GetVelocity();
    Velocity.Z = 0.f;
    return Velocity.Size();
}

void AAS_Character::Die()
{
    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->DisableMovement();
    }

    if (DeathAnimation)
    {
        GetMesh()->PlayAnimation(DeathAnimation, false);
    }
    else
    {
        // Ragdoll
        if (GetMesh())
        {
            GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
            GetMesh()->SetSimulatePhysics(true);
        }
    }

    if (CombatComponent && CombatComponent->GetEquippedWeapon())
    {
        CombatComponent->GetEquippedWeapon()->Destroy();
    }
}

AAS_BaseWeapon* AAS_Character::GetEquippedWeapon() const
{
    if (!CombatComponent) return nullptr;
    return CombatComponent->GetEquippedWeapon();
}

void AAS_Character::CrosshairActivate(const FVector& StartLocation, const FVector& EndLocation)
{
    ClearCrosshair();
    DrawCrosshair(StartLocation, EndLocation);
}

void AAS_Character::CrosshairDeactivate()
{
    ClearCrosshair();
}

void AAS_Character::DrawCrosshair(const FVector& StartLocation, const FVector& EndLocation)
{
    if (!CrosshairComponent || !SplineMesh || !SplineMaterial) return;

    CrosshairComponent->AddSplinePointAtIndex(StartLocation, 0, ESplineCoordinateSpace::World);
    CrosshairComponent->AddSplinePointAtIndex(EndLocation, 1, ESplineCoordinateSpace::World);

    CrosshairComponent->SetSplinePointType(1, ESplinePointType::Linear);

    SplineMeshComponent = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), true, FTransform(), false));
    if (!SplineMeshComponent) return;

    SplineMeshComponent->SetMobility(EComponentMobility::Movable);

    if (SplineMesh && SplineMaterial)
    {
        SplineMeshComponent->SetStaticMesh(SplineMesh);
        SplineMeshComponent->SetMaterial(0, SplineMaterial);
        SplineMeshComponent->CastShadow = 0;
    }

    SplineMeshComponent->SetStartScale(FVector2D(CrosshairDepth, CrosshairDepth));
    SplineMeshComponent->SetEndScale(FVector2D(CrosshairDepth, CrosshairDepth));

    SplineMeshComponent->SetStartAndEnd(                                                //
        StartLocation,                                                                  //
        CrosshairComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World),  //
        EndLocation,                                                                    //
        CrosshairComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::World)   //
    );
}

void AAS_Character::ClearCrosshair()
{
    if (!CrosshairComponent) return;
    CrosshairComponent->ClearSplinePoints();

    if (!SplineMeshComponent) return;
    SplineMeshComponent->DestroyComponent();
}

void AAS_Character::UpdateHUDAmmoInfo()
{
    if (IsLocallyControlled())
    {
        PlayerController = (!PlayerController) ? GetController<AAS_PlayerController>() : PlayerController;
        if (!PlayerController || !CombatComponent || !CombatComponent->GetEquippedWeapon()) return;

        PlayerController->SetAmmoInfo(CombatComponent->GetEquippedWeapon()->GetAmmoInfoText());
    }
}
