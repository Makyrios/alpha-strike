// AlphaStrike by Team #1. AlphaNova courses🤙

#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"
#include "Net/UnrealNetwork.h"

void AAS_TeamDeathmatchPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAS_TeamDeathmatchPlayerState, Team);
}
