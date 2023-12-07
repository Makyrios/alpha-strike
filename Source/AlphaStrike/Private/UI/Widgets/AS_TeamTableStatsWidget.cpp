// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_TeamTableStatsWidget.h"
#include "Components/VerticalBox.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"
#include "GameFramework/GameState.h"
#include "UI/Widgets/AS_PlayerStatsWidget.h"

void UAS_TeamTableStatsWidget::AddPlayerStatsToTable(AAS_BasePlayerState* PlayerToAdd)
{
    if (!PlayerToAdd || !PlayersTeamA || !PlayersTeamB) return;

    UAS_PlayerStatsWidget* NewPlayerStats = CreateWidget<UAS_PlayerStatsWidget>(this, PlayerStatsClass);
    PlayerStatesInTable.Add(PlayerToAdd);

    if (NewPlayerStats)
    {
        NewPlayerStats->SetPlayerState(PlayerToAdd);
    }
    if (const AAS_TeamDeathmatchPlayerState* TeamPlayerState = Cast<AAS_TeamDeathmatchPlayerState>(PlayerToAdd))
    {
        if (TeamPlayerState->GetTeam() == ETeams::TEAM_A)
        {
            PlayersTeamA->AddChild(NewPlayerStats);
        }
        else if (TeamPlayerState->GetTeam() == ETeams::TEAM_B)
        {
            PlayersTeamB->AddChild(NewPlayerStats);
        }
    }
}
