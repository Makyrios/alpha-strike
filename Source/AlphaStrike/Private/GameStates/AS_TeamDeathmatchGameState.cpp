// AlphaStrike by Team #1. AlphaNova courses🤙


#include "GameStates/AS_TeamDeathmatchGameState.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"

AAS_TeamDeathmatchGameState::AAS_TeamDeathmatchGameState() 
{
    TeamsInfo.Add(ETeams::TEAM_A, FTeamInfo());
    TeamsInfo.Add(ETeams::TEAM_B, FTeamInfo());
}

void AAS_TeamDeathmatchGameState::AddPlayerToTeam(APlayerState* Player, ETeams Team)
{
    if (!Player) return;

    FTeamInfo* TeamInfo = TeamsInfo.Find(Team);
    TeamInfo->Members.Add(Player);

    if (AAS_TeamDeathmatchPlayerState* TeamPlayer = Cast<AAS_TeamDeathmatchPlayerState>(Player)) 
    {
        TeamInfo->Score += TeamPlayer->GetKills();
    }
}

void AAS_TeamDeathmatchGameState::AddScoreToTeam(AController* ScoredPlayer)
{
    if (!ScoredPlayer) return;

    if (AAS_TeamDeathmatchPlayerState* PlayerState = ScoredPlayer->GetPlayerState<AAS_TeamDeathmatchPlayerState>())
    {
        FTeamInfo* TeamInfo = TeamsInfo.Find(PlayerState->GetTeam());
        TeamInfo->Score += 1;
    }
}

int32 AAS_TeamDeathmatchGameState::GetTeamScore(ETeams Team) const
{
    FTeamInfo const* TeamInfo = TeamsInfo.Find(Team);
    return TeamInfo->Score;
}