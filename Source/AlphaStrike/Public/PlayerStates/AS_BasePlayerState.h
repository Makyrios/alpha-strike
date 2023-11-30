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
    UPROPERTY(ReplicatedUsing = OnRep_Kills, VisibleAnywhere, BlueprintReadOnly, Category = "AS|Stats")
    int32 Kills = 0;

    UPROPERTY(ReplicatedUsing = OnRep_Deaths, BlueprintReadOnly, Category = "AS|Stats")
    int32 Deaths = 0;

private:
    UFUNCTION()
    void OnRep_Kills();

    UFUNCTION()
    void OnRep_Deaths();
};
