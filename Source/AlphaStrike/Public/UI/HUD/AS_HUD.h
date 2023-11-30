// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AS_HUD.generated.h"

class UAS_MenuWidget;
class AAS_PlayerController;
class UAS_TableStatsWidget;
class UAS_HUDWidget;
class UAS_PauseWidget;
class UProgressBar;

UCLASS()
class ALPHASTRIKE_API AAS_HUD : public AHUD
{
    GENERATED_BODY()
public:
    void SetHealthBarPercent(float Percent);
    void SetShieldBarPercent(float Percent);
    void SetTimeRemaining(float RemainingTimeInSeconds);

    void Tick(float DeltaTime) override;
    
    void ShowStatsTable();
    void HideStatsTable();

    void Pause(bool bPause);
    void ExitToMenu();

    void ShowStartGameWidget(float StartDelayTime);

protected:
    void BeginPlay() override;

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
};
