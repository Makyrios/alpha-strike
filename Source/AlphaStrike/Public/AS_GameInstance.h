// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AS_GameInstance.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    FORCEINLINE void SetPlayerName(FText NewName) { PlayerName = NewName; }
    FORCEINLINE FText GetPlayerName() { return PlayerName; }

public:
    FText PlayerName;

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName TeamDeathmatchMapName = TEXT("BotsMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName DeathmatchMapName = TEXT("BotsMap_2");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName MultDeathmatchMapName = TEXT("PlayersMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName MenuMapName = TEXT("MenuMap");
};
