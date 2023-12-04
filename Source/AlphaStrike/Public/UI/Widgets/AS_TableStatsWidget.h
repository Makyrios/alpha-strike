// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_TableStatsWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class AAS_BasePlayerState;

UCLASS()
class ALPHASTRIKE_API UAS_TableStatsWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetPlayerNumber(int32 PlayerNumber);
    void SetMapName(FText MapName);

protected:
    void NativeOnInitialized() override;
    void UpdatePlayers();
    virtual void AddPlayerStatsToTable(AAS_BasePlayerState* PlayerToAdd);

protected:
    UPROPERTY()
    TArray<AAS_BasePlayerState*> PlayerStatesInTable;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> PlayerStatsClass;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* PlayersNumberBox;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* MapNameBox;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UVerticalBox* Players;

    FTimerHandle UpdateTimer;
    float UpdateDelay = 2.0f;
};
