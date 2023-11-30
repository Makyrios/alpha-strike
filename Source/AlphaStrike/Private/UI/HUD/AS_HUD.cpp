// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/HUD/AS_HUD.h"
#include "UI/Widgets/AS_HUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStates/AS_BaseGameState.h"
#include "UI/Widgets/AS_TableStatsWidget.h"
#include "UI/Widgets/AS_PauseWidget.h"
#include "Controllers/AS_PlayerController.h"
#include <UI/Widgets/AS_StartGameWidget.h>

void AAS_HUD::BeginPlay()
{
    Super::BeginPlay();

    HUDWidget = AddWidget<UAS_HUDWidget>(HUDWidgetClass);

    TableStatsWidget = AddWidget<UAS_TableStatsWidget>(StatsTableClass);
    if (TableStatsWidget)
    {
        TableStatsWidget->SetVisibility(ESlateVisibility::Collapsed);

        const FString MapName = UGameplayStatics::GetCurrentLevelName(this);
        TableStatsWidget->SetMapName(FText::FromString(MapName));
    }

    PauseWidget = AddWidget<UAS_PauseWidget>(PauseWidgetClass);
    if (PauseWidget)
    {
        PauseWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void AAS_HUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetupTableWidget();
}

void AAS_HUD::SetupTableWidget()
{
    // TODO refactor from tick to bind finction

    if (!TableStatsWidget) return;
    const int32 NumberOfPlayers = UGameplayStatics::GetNumPlayerStates(this);
    TableStatsWidget->SetPlayerNumber(NumberOfPlayers);
}

void AAS_HUD::ShowStatsTable()
{
    if (!TableStatsWidget) return;
    TableStatsWidget->SetVisibility(ESlateVisibility::Visible);
}

void AAS_HUD::HideStatsTable()
{
    if (!TableStatsWidget) return;
    TableStatsWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AAS_HUD::Pause(bool bPause)
{
    if (!PauseWidget) return;

    if (bPause)
    {
        PauseWidget->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        PauseWidget->SetVisibility(ESlateVisibility::Collapsed);

        AAS_PlayerController* OwnerController = Cast<AAS_PlayerController>(GetOwningPlayerController());
        if (!OwnerController) return;

        OwnerController->UnPause();
    }
}

void AAS_HUD::ExitToMenu()
{
    AAS_PlayerController* OwnerController = Cast<AAS_PlayerController>(GetOwningPlayerController());
    if (!OwnerController) return;

    OwnerController->ExitToMenu();
}

void AAS_HUD::ShowStartGameWidget(float StartDelayTime)
{
    UAS_StartGameWidget* StartGameWidget = AddWidget<UAS_StartGameWidget>(StartGameWidgetClass);

    if (StartGameWidget)
    {
        StartGameWidget->InitializeWidget(StartDelayTime);
    }
}

void AAS_HUD::SetHealthBarPercent(float Percent)
{
    if (!HUDWidget) return;

    if (UAS_HUDWidget* CurrentHUDWidget = Cast<UAS_HUDWidget>(HUDWidget))
    {
        CurrentHUDWidget->SetHealth(Percent);
    }
}

void AAS_HUD::SetShieldBarPercent(float Percent)
{
    if (!HUDWidget) return;

    if (UAS_HUDWidget* CurrentHUDWidget = Cast<UAS_HUDWidget>(HUDWidget))
    {
        CurrentHUDWidget->SetShield(Percent);
    }
}

void AAS_HUD::SetTimeRemaining(float RemainingTimeInSeconds)
{
    if (!HUDWidget) return;

    if (UAS_HUDWidget* CurrentHUDWidget = Cast<UAS_HUDWidget>(HUDWidget))
    {
        CurrentHUDWidget->SetTimerText(RemainingTimeInSeconds);
    }
}

 void AAS_HUD::SetAmmoInfo(FText NewAmmoInfo) 
 {
    if (!HUDWidget) return;

    if (UAS_HUDWidget* CurrentHUDWidget = Cast<UAS_HUDWidget>(HUDWidget))
    {
        CurrentHUDWidget->SetAmmoInfoText(NewAmmoInfo);
    }
 }

template <typename T>
T* AAS_HUD::AddWidget(TSubclassOf<UUserWidget> WidgetToAdd)
{
    if (!GetOwningPlayerController() || !WidgetToAdd) return nullptr;

    T* NewWidget = CreateWidget<T>(GetOwningPlayerController(), WidgetToAdd);
    if (!NewWidget) return nullptr;

    NewWidget->AddToViewport();

    return NewWidget;
}
