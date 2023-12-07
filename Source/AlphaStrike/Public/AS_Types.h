// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "AS_Types.generated.h"

// Health
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeadDelegate, class AActor*, DeadActor, class AController*, InstigatedBy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, class AActor*, ChangedActor, bool, bDamage);

// Weapon
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    EWT_Rifle UMETA(DisplayName = "Rifle"),
    EWT_Pistol UMETA(DisplayName = "Pistol"),
    EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
    EWT_Empty UMETA(DisplayName = "Empty"),

    EWT_MAX UMETA(DisplayName = "MAX"),
};

USTRUCT(BlueprintType)
struct FAmmoInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    bool bFiniteAmmo = true;

    UPROPERTY()
    int32 CurrentAmmo = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bFiniteAmmo"))
    int32 MaxAmmoInClip = 10;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bFiniteAmmo"))
    int32 AmmoInBag = 30;
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

USTRUCT(BlueprintType)
struct FTeamSpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 NumberOfPawns = 4;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APawn> PawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<APawn> HeavyPawnClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<AController> ControllerClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FLinearColor TeamColor = FLinearColor::Black;
};

USTRUCT(BlueprintType)
struct FTeamInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<AController*> Members;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 Score = 0;
};

UENUM(BlueprintType)
enum class ETeams
{
    TEAM_A UMETA(DisplayName = "Team A"),
    TEAM_B UMETA(DisplayName = "Team B")
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
