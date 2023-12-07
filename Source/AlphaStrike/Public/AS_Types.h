// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "AS_Types.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_Rifle UMETA(DisplayName = "Rifle"),
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
    EWT_Empty UMETA(DisplayName = "Empty"),

    EWT_MAX UMETA(DisplayName = "MAX"),
};

// Animation
UENUM(BlueprintType) enum class ETurningInPlace : uint8
{
    ETIP_Right UMETA(DisplayName = "Turn right"),
    ETIP_Left UMETA(DisplayName = "Turn left"),
    ETIP_NotTurning UMETA(DisplayName = "Not turning"),

    ETIP_MAX UMETA(DisplayName = "MAX")
};

// GameMode
USTRUCT(BlueprintType)
struct FPawnSpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 NumberOfPawns = 1;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintBaseOnly))
    TSubclassOf<APawn> PawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintBaseOnly))
    TSubclassOf<APawn> HeavyPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BlueprintBaseOnly))
    TSubclassOf<AController> ControllerClass;
};

// Menu
UENUM(BlueprintType) enum class EMenuAction : uint8
{
    EMA_Deathmatch UMETA(DisplayName = "Deathmatch"),
    EMA_TeamDeathmatch UMETA(DisplayName = "TeamDeathmatch"),
    EMA_Host UMETA(DisplayName = "Host"),
    EMA_Join UMETA(DisplayName = "Join"),

    EMA_MAX UMETA(DisplayName = "MAX")
};
