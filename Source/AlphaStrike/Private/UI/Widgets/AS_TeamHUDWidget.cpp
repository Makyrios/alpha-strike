// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_TeamHUDWidget.h"
#include "Components/TextBlock.h"

void UAS_TeamHUDWidget::SetTeamAScore(int32 NewScore)
{
    if (TeamAScoreText)
    {
        const FString StringScore = FString::FromInt(NewScore);
        TeamAScoreText->SetText(FText::FromString(StringScore));
    }
}
void UAS_TeamHUDWidget::SetTeamBScore(int32 NewScore)
{
    if (TeamBScoreText)
    {
        const FString StringScore = FString::FromInt(NewScore);
        TeamBScoreText->SetText(FText::FromString(StringScore));
    }
}
