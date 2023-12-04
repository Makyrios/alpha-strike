// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameStates/AS_BaseGameState.h"
#include "UI/HUD/AS_HUD.h"
#include "GameModes/AS_BaseGameMode.h"

AAS_BaseGameState::AAS_BaseGameState()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAS_BaseGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetTimeForHUD();
}

void AAS_BaseGameState::SetTimeForHUD()
{
    AAS_BaseGameMode const* CustomGameMode = Cast<AAS_BaseGameMode>(GetDefaultGameMode());

    if (!CustomGameMode) return;

    float TimeLimit = CustomGameMode->GetTimeLimit();
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        if (AHUD* PlayerHUD = Iterator->Get()->GetHUD())
        {
            AAS_HUD* CustomHUD = Cast<AAS_HUD>(PlayerHUD);
            if (CustomHUD && CustomGameMode->IsGameStarted())
            {
                CustomHUD->SetTimeRemaining(TimeLimit - GetServerWorldTimeSeconds() + CustomGameMode->GetDelayBeforeStart());
            }
        }
    }
}
