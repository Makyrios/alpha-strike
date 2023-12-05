// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Controllers/AS_MenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AS_GameInstance.h"
#include "UI/HUD/AS_MenuHUD.h"
#include "Sound/SoundCue.h"

void AAS_MenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    auto HUD = GetHUD<AAS_MenuHUD>();
    if (!HUD) return;

    HUD->AddMenuWidget();

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    SetInputMode(InputModeData);
    SetShowMouseCursor(true);

    if (MenuSound && GetWorld())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), MenuSound);
    }
}

void AAS_MenuPlayerController::CreateDeathmatchGame() 
{
    const auto GameInstance = GetGameInstance<UAS_GameInstance>();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->DeathmatchMapName);
}

void AAS_MenuPlayerController::CreateTeamDeathmatchGame() 
{
    const auto GameInstance = GetGameInstance<UAS_GameInstance>();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->TeamDeathmatchMapName);
}

void AAS_MenuPlayerController::CreateDeathmatchHostGame() 
{
    const auto GameInstance = GetGameInstance<UAS_GameInstance>();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->MultDeathmatchMapName, true, TEXT("listen"));
}

void AAS_MenuPlayerController::JoinGame(const FText& IP_Address) 
{
    const FName Address = *IP_Address.ToString();
    UGameplayStatics::OpenLevel(this, Address);
}
