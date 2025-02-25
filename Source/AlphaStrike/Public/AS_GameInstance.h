﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AS_GameInstance.generated.h"

class USoundClass;

UCLASS()
class ALPHASTRIKE_API UAS_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    void Init() override;

    UFUNCTION()
    void BeginLoadingScreen(const FString& InMapName);

    void SetMasterSoundVolume(float Volume);
    float GetMasterSoundVolume() const;

    FORCEINLINE void SetPlayerName(FText NewName) { PlayerName = NewName; }
    FORCEINLINE FText GetPlayerName() const { return PlayerName; }

public:
    FText PlayerName = FText::FromString(TEXT("Player"));

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName TeamDeathmatchMapName = TEXT("BotsMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName DeathmatchMapName = TEXT("BotsMap_2");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName MultDeathmatchMapName = TEXT("PlayersMap_1");

    UPROPERTY(EditDefaultsOnly, Category = "AS|GameInfo")
    FName MenuMapName = TEXT("MenuMap");

    UPROPERTY(EditDefaultsOnly, Category = "AS|Sound")
    USoundClass* MasterSoundClass;

    UPROPERTY(EditDefaultsOnly, Category = "AS|LoadingScreen")
    TSubclassOf<UUserWidget> LoadingScreenClass;
};
