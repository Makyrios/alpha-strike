// AlphaStrike by Team #1. AlphaNova courses🤙


#include "GameStates/AS_DeathmatchGameState.h"
#include "PlayerStates/AS_DeathmatchPlayerState.h"

void AAS_DeathmatchGameState::UpdateMaxKillsAndWinningPlayer(AController* PlayerController)
{
    if (!PlayerController) return;

    auto PlayerState = PlayerController->GetPlayerState<AAS_DeathmatchPlayerState>();
    if (PlayerState)
    {
        if (PlayerState->GetKills() > MaxPlayerKills)
        {
            MaxPlayerKills = PlayerState->GetKills();
            WinningPlayer = PlayerController;
        }
    }
}
