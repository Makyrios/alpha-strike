// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/HUD/AS_MenuHUD.h"
#include "UI/Widgets/AS_MenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Controllers/AS_MenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Saves/AS_PlayerSavedInfo.h"
#include "AS_GameInstance.h"

AAS_MenuHUD::AAS_MenuHUD()
{
    USaveGame* Save = UGameplayStatics::LoadGameFromSlot("Player Info", 0);
    if (UAS_PlayerSavedInfo* PlayerInfo = Cast <UAS_PlayerSavedInfo>(Save)) 
    {
        if (MenuWidget)
        {
            MenuWidget->SetNameText(PlayerInfo->Name);
        }
    }
    
}

void AAS_MenuHUD::AddMenuWidget()
{
    if (!GetOwningPlayerController() || !MenuWidgetClass || MenuWidget) return;

    MenuWidget = CreateWidget<UAS_MenuWidget>(GetOwningPlayerController(), MenuWidgetClass);
    if (!MenuWidget) return;

    MenuWidget->AddToViewport();

    if (!MenuWidget->SingleDeathmatchButton || !MenuWidget->HostButton || !MenuWidget->JoinButton) return;

    MenuWidget->SingleDeathmatchButton->OnClicked.AddDynamic(this, &AAS_MenuHUD::OnSingleDeathmatchButtonClicked);
    MenuWidget->TeamDeathmatchButton->OnClicked.AddDynamic(this, &AAS_MenuHUD::OnTeamDeathmatchButtonClicked);
    MenuWidget->HostButton->OnClicked.AddDynamic(this, &AAS_MenuHUD::OnHostButtonClicked);
    MenuWidget->JoinButton->OnClicked.AddDynamic(this, &AAS_MenuHUD::OnJoinButtonClicked);
    MenuWidget->ApplyNameButton->OnClicked.AddDynamic(this, &AAS_MenuHUD::OnApplyNameButtonClicked);
}

void AAS_MenuHUD::OnSingleDeathmatchButtonClicked()
{
    if (!GetPlayerController()) return;
    GetPlayerController()->CreateDeathmatchGame();
}

void AAS_MenuHUD::OnTeamDeathmatchButtonClicked() 
{
    if (!GetPlayerController()) return;
    GetPlayerController()->CreateTeamDeathmatchGame();
}

void AAS_MenuHUD::OnHostButtonClicked()
{
    if (!GetPlayerController()) return;
    GetPlayerController()->CreateDeathmatchHostGame();
}

void AAS_MenuHUD::OnJoinButtonClicked()
{
    if (!GetPlayerController() || !MenuWidget || !MenuWidget->IP_AddressBox) return;
    GetPlayerController()->JoinGame(MenuWidget->IP_AddressBox->GetText());
}

void AAS_MenuHUD::OnApplyNameButtonClicked()
{
    UAS_GameInstance* GameInstance = Cast <UAS_GameInstance>(GetGameInstance());

    if (!MenuWidget || !GameInstance) return;

    FText NewName = MenuWidget->GetNameText();
    GameInstance->SetPlayerName(NewName);
}

AAS_MenuPlayerController* AAS_MenuHUD::GetPlayerController()
{
    AS_PlayerController = (!AS_PlayerController) ? Cast<AAS_MenuPlayerController>(GetOwningPlayerController()) : AS_PlayerController;
    return AS_PlayerController;
}


