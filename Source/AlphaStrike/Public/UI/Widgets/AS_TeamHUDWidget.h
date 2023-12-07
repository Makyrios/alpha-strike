// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/AS_HUDWidget.h"
#include "AS_TeamHUDWidget.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_TeamHUDWidget : public UAS_HUDWidget
{
    GENERATED_BODY()

public:
    void SetTeamAScore(int32 NewScore);
    void SetTeamBScore(int32 NewScore);

protected:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* TeamAScoreText;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UTextBlock* TeamBScoreText;
};
