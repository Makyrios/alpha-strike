// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_PlayerStatsWidget.generated.h"

class AAS_BasePlayerState;
class UTextBlock;
class UImage;

UCLASS()
class ALPHASTRIKE_API UAS_PlayerStatsWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetPlayerState(AAS_BasePlayerState* NewPlayerState);
    void SetPlayerName(FText PlayerName);
    void SetKills(int32 Kills);
    void SetDeaths(int32 Deaths);
    void SetPing(int32 Ping);

protected:
    void NativeOnInitialized() override;

    void UpdatePlayerStats();

private:
    UPROPERTY()
    AAS_BasePlayerState* PlayerState;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* PlayerNameBox;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* KillsBox;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* DeathsBox;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* PingBox;

    FTimerHandle UpdateTimer;
    float UpdateDelay = 0.2f;
};
