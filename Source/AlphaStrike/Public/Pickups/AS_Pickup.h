// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AS_Pickup.generated.h"

class USphereComponent;

UCLASS(Abstract)
class ALPHASTRIKE_API AAS_Pickup : public AActor
{
    GENERATED_BODY()

public:
    AAS_Pickup();

public:
    virtual void Tick(float DeltaTime) override;

    bool CouldBeTaken() const;

    void Spawn();
    void Despawn();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AS|Components", meta = (AllowPrivateAccess = "true"))
    USphereComponent* Sphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Pickup", meta = (AllowPrivateAccess = "true"))
    bool bRotateMesh = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Pickup", meta = (AllowPrivateAccess = "true", EditCondition = "bRotateMesh"))
    float RotationSpeed = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AS|Spawn", meta = (AllowPrivateAccess = "true"))
    bool bSpawnImmediately = true;

    UPROPERTY(
        EditAnywhere, BlueprintReadWrite, Category = "AS|Spawn", meta = (AllowPrivateAccess = "true", EditCondition = "bSpawnImmediately"))
    float RespawnTime = 10.f;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnSphereEndOverlap(
        UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    virtual void PickupInteract(TArray<AActor*>& InteractedActors);

private:
    void RotateMesh(float DeltaTime);

private:
    TArray<AActor*> OverlappingActors;

    FTimerHandle RespawnTimer;
};
