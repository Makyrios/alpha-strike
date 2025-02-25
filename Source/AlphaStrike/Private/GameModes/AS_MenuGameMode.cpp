﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#include "GameModes/AS_MenuGameMode.h"
#include "Controllers/AS_MenuPlayerController.h"
#include "UI/HUD/AS_MenuHUD.h"

AAS_MenuGameMode::AAS_MenuGameMode() 
{
    PlayerControllerClass = AAS_MenuPlayerController::StaticClass();
    HUDClass = AAS_MenuHUD::StaticClass();
}
