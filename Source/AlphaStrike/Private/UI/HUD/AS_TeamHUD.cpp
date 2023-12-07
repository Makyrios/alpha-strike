// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/HUD/AS_TeamHUD.h"
#include "UI/Widgets/AS_TeamHUDWidget.h"

void AAS_TeamHUD::SetTeamAScore(int32 NewScore)
{
    if (!HUDWidget) return;

    if (UAS_TeamHUDWidget* TeamHUDWidget = Cast<UAS_TeamHUDWidget>(HUDWidget))
    {
        TeamHUDWidget->SetTeamAScore(NewScore);
    }
}
void AAS_TeamHUD::SetTeamBScore(int32 NewScore)
{
    if (!HUDWidget) return;

    if (UAS_TeamHUDWidget* TeamHUDWidget = Cast<UAS_TeamHUDWidget>(HUDWidget))
    {
        TeamHUDWidget->SetTeamBScore(NewScore);
    }
}
