// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AS_BasePlayerState.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_BasePlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FORCEINLINE void AddKill() { ++Kills; }
    FORCEINLINE void AddDeath() { ++Deaths; }

    FORCEINLINE int32 GetKills() const { return Kills; }
    FORCEINLINE int32 GetDeaths() const { return Deaths; }

protected:
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "AS|Stats")
    int32 Kills = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "AS|Stats")
    int32 Deaths = 0;
};
