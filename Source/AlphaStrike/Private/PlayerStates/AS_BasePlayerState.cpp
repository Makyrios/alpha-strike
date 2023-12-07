// AlphaStrike by Team #1. AlphaNova courses🤙

#include "PlayerStates/AS_BasePlayerState.h"
#include "Net/UnrealNetwork.h"

void AAS_BasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAS_BasePlayerState, Kills);
    DOREPLIFETIME(AAS_BasePlayerState, Deaths);
}
