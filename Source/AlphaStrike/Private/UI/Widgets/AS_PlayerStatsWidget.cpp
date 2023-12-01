// AlphaStrike by Team #1. AlphaNova courses🤙


#include "UI/Widgets/AS_PlayerStatsWidget.h"
#include "Components/TextBlock.h"
#include "PlayerStates/AS_BasePlayerState.h"

void UAS_PlayerStatsWidget::NativeOnInitialized() 
{
    Super::NativeOnInitialized();

    GetWorld()->GetTimerManager().SetTimer(UpdateTimer, this, &UAS_PlayerStatsWidget::UpdatePlayerStats, UpdateDelay, true, 0.0f);
}

void UAS_PlayerStatsWidget::UpdatePlayerStats() 
{
    if (!PlayerState) return;

    FString PlayerName = PlayerState->GetPlayerName();
    SetPlayerName(FText::FromString(PlayerName));
    SetKills(PlayerState->GetKills());
    SetDeaths(PlayerState->GetDeaths());
    SetPing(PlayerState->GetPingInMilliseconds());
}

 void UAS_PlayerStatsWidget::SetPlayerState(AAS_BasePlayerState* NewPlayerState) 
 {
     if (!NewPlayerState) return;
     PlayerState = NewPlayerState;
 }

void UAS_PlayerStatsWidget::SetPlayerName(FText PlayerName) 
{
    PlayerNameBox->SetText(PlayerName);
}
void UAS_PlayerStatsWidget::SetKills(int32 Kills) 
{
    FString KilllsString = FString::FromInt(Kills);
    KillsBox->SetText(FText::FromString(KilllsString));
}
void UAS_PlayerStatsWidget::SetDeaths(int32 Deaths) 
{
    FString DeathsString = FString::FromInt(Deaths);
    DeathsBox->SetText(FText::FromString(DeathsString));
}
void UAS_PlayerStatsWidget::SetPing(int32 Ping) 
{
    FString PingString = FString::FromInt(Ping);
    PingBox->SetText(FText::FromString(PingString));
}
