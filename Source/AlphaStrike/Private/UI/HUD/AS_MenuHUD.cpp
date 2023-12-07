// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/HUD/AS_MenuHUD.h"
#include "UI/Widgets/AS_MenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Controllers/AS_MenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "AS_GameInstance.h"
#include "Animation/WidgetAnimation.h"

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

void AAS_MenuHUD::HandleMenuAction()
{
    if (!GetPlayerController()) return;

    switch (MenuAction)
    {
        case EMenuAction::EMA_Deathmatch:  //
            GetPlayerController()->CreateDeathmatchGame();
            break;
        case EMenuAction::EMA_TeamDeathmatch:  //
            GetPlayerController()->CreateTeamDeathmatchGame();
            break;
        case EMenuAction::EMA_Host:  //
            GetPlayerController()->CreateDeathmatchHostGame();
            break;
        case EMenuAction::EMA_Join:  //
            if (MenuWidget && MenuWidget->IP_AddressBox)
            {
                GetPlayerController()->JoinGame(MenuWidget->IP_AddressBox->GetText());
            }
            break;
    }
}

void AAS_MenuHUD::OnSingleDeathmatchButtonClicked()
{
    if (!MenuWidget) return;

    MenuWidget->PlayJoinAnimation();
    MenuAction = EMenuAction::EMA_Deathmatch;
    MenuWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AAS_MenuHUD::OnTeamDeathmatchButtonClicked()
{
    if (!MenuWidget) return;

    MenuWidget->PlayJoinAnimation();
    MenuAction = EMenuAction::EMA_TeamDeathmatch;
    MenuWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AAS_MenuHUD::OnHostButtonClicked()
{
    if (!MenuWidget) return;

    MenuWidget->PlayJoinAnimation();
    MenuAction = EMenuAction::EMA_Host;
    MenuWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AAS_MenuHUD::OnJoinButtonClicked()
{
    if (!MenuWidget) return;

    MenuWidget->PlayJoinAnimation();
    MenuAction = EMenuAction::EMA_Join;
    MenuWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void AAS_MenuHUD::OnApplyNameButtonClicked()
{
    UAS_GameInstance* GameInstance = Cast<UAS_GameInstance>(GetGameInstance());

    if (!MenuWidget || !GameInstance) return;

    const FText NewName = MenuWidget->GetNameText();
    GameInstance->SetPlayerName(NewName);
}

AAS_MenuPlayerController* AAS_MenuHUD::GetPlayerController()
{
    AS_PlayerController = (!AS_PlayerController) ? Cast<AAS_MenuPlayerController>(GetOwningPlayerController()) : AS_PlayerController;
    return AS_PlayerController;
}
