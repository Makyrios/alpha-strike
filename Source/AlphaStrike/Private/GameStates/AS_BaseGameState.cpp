// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameStates/AS_BaseGameState.h"
#include "UI/HUD/AS_HUD.h"
#include "GameModes/AS_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Controllers/AS_PlayerController.h"

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
    AAS_BaseGameMode* CustomGameMode = Cast<AAS_BaseGameMode>(UGameplayStatics::GetGameMode(this));
    if (!CustomGameMode) return;

    const float TimeLimit = CustomGameMode->GetTimeLimit();
    for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
    {
        AAS_PlayerController* Controller = Cast<AAS_PlayerController>(*Iterator);
        if (Controller)
        {
            Controller->SetTimeRemaining(TimeLimit - GetServerWorldTimeSeconds() + CustomGameMode->GetDelayBeforeStart());
        }
    }
}
