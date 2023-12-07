// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_TableStatsWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "PlayerStates/AS_BasePlayerState.h"
#include "GameFramework/GameState.h"
#include "UI/Widgets/AS_PlayerStatsWidget.h"

void UAS_TableStatsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &UAS_TableStatsWidget::UpdatePlayers, UpdateDelay, true, 0.0f);
}

void UAS_TableStatsWidget::UpdatePlayers()
{
    const AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
    const TArray<APlayerState*> PlayerStates = GameState->PlayerArray;

    for (int i = 0; i < PlayerStates.Num(); ++i)
    {
        if (AAS_BasePlayerState* PlayerState = Cast<AAS_BasePlayerState>(PlayerStates[i]))
        {
            if (PlayerStatesInTable.Find(PlayerState) == INDEX_NONE)
            {
                AddPlayerStatsToTable(PlayerState);
            }
        }
    }
}

void UAS_TableStatsWidget::AddPlayerStatsToTable(AAS_BasePlayerState* PlayerToAdd)
{
    if (!PlayerToAdd) return;

    UAS_PlayerStatsWidget* NewPlayerStats = CreateWidget<UAS_PlayerStatsWidget>(this, PlayerStatsClass);
    if (!NewPlayerStats || !Players) return;

    PlayerStatesInTable.Add(PlayerToAdd);
    NewPlayerStats->SetPlayerState(PlayerToAdd);
    Players->AddChild(NewPlayerStats);
}

void UAS_TableStatsWidget::SetPlayerNumber(int32 PlayerNumber)
{
    if (PlayersNumberBox)
    {
        const FString PlayerNumberString = FString::FromInt(PlayerNumber);
        PlayersNumberBox->SetText(FText::FromString(PlayerNumberString));
    }
}

void UAS_TableStatsWidget::SetMapName(FText MapName)
{
    if (MapNameBox)
    {
        MapNameBox->SetText(MapName);
    }
}
