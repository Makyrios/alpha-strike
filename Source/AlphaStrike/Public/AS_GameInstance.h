// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AS_GameInstance.generated.h"

class UAS_PlayerSavedInfo;

UCLASS()
class ALPHASTRIKE_API UAS_GameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    void Init() override;

    FORCEINLINE UAS_PlayerSavedInfo* GetPlayerInfo() { return PlayerInfo; }
    FORCEINLINE void SetPlayerName(FText NewName) { PlayerName = NewName; }
    FORCEINLINE FText GetPlayerName() { return PlayerName; }

public:
    FText PlayerName;
    UAS_PlayerSavedInfo* PlayerInfo;

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName BotsMapName = TEXT("BotsMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName PlayersMapName = TEXT("PlayersMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName MenuMapName = TEXT("MenuMap");
};
