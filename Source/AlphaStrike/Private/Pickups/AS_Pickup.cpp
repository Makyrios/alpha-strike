// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Pickups/AS_Pickup.h"
#include "Components/SphereComponent.h"

AAS_Pickup::AAS_Pickup()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = StaticMesh;

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(StaticMesh);
}

void AAS_Pickup::BeginPlay()
{
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAS_Pickup::OnSphereOverlap);
    Sphere->OnComponentEndOverlap.AddDynamic(this, &AAS_Pickup::OnSphereEndOverlap);

    if (!bSpawnImmediately)
    {
        Despawn();
    }
}

void AAS_Pickup::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    OverlappingActors.Add(OtherActor);
}

void AAS_Pickup::OnSphereEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    OverlappingActors.Remove(OtherActor);
}

void AAS_Pickup::PickupInteract(TArray<AActor*>& InteractedActors)
{
    GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &AAS_Pickup::Spawn, RespawnTime, false);
}

void AAS_Pickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OverlappingActors.Num() > 0)
    {
        PickupInteract(OverlappingActors);
    }

    if (bRotateMesh)
    {
        RotateMesh(DeltaTime);
    }
}

bool AAS_Pickup::CouldBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimer);
}

void AAS_Pickup::Spawn()
{
    StaticMesh->SetVisibility(true);
    Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAS_Pickup::Despawn()
{
    StaticMesh->SetVisibility(false);
    Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAS_Pickup::RotateMesh(float DeltaTime)
{
    if (StaticMesh != nullptr)
    {
        StaticMesh->AddLocalRotation(FRotator(0, RotationSpeed * DeltaTime, 0));
    }
}
