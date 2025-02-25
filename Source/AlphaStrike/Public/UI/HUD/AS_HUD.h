﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AS_HUD.generated.h"

class UAS_TableStatsWidget;
class UAS_HUDWidget;
class UAS_PauseWidget;
class UAS_DamageWidget;

UCLASS()
class ALPHASTRIKE_API AAS_HUD : public AHUD
{
    GENERATED_BODY()

public:
    virtual void PostInitializeComponents() override;

    void SetHUDWidgetVisibility(ESlateVisibility InVisibility);

    void SetHealthBarPercent(float Percent);
    void SetShieldBarPercent(float Percent);
    void SetTimeRemaining(float RemainingTimeInSeconds);
    void SetAmmoInfo(FText NewAmmoInfo);
    void SetScoreGoal(int32 ScoreGoal);

    virtual void SetTeamAScore(int32 NewScore){};
    virtual void SetTeamBScore(int32 NewScore){};

    void Tick(float DeltaTime) override;
    
    void ShowStatsTable();
    void HideStatsTable();

    void PlayDamageAnimation();

    void Pause(bool bPause);
    void ExitToMenu();

    void ShowStartGameWidget(float StartDelayTime);

    void ShowWonWidget();
    void ShowLoseWidget();

    void UpdateInventoryInfo();

protected:
    template<typename T>
    T* AddWidget(TSubclassOf<UUserWidget> WidgetToAdd);

    void SetupTableWidget();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> HUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> StartGameWidgetClass;

    UPROPERTY()
    UAS_HUDWidget* HUDWidget;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> StatsTableClass;

    UPROPERTY()
    UAS_TableStatsWidget* TableStatsWidget;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY()
    UAS_PauseWidget* PauseWidget;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> WonWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> LoseWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "AS|Widgets")
    TSubclassOf<UUserWidget> DamageWidgetClass;

    UPROPERTY()
    UAS_DamageWidget* DamageWidget;
};
