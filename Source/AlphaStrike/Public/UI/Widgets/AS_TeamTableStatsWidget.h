// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/AS_TableStatsWidget.h"
#include "AS_TeamTableStatsWidget.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_TeamTableStatsWidget : public UAS_TableStatsWidget
{
    GENERATED_BODY()

protected:
    void AddPlayerStatsToTable(AAS_BasePlayerState* PlayerToAdd) override;

private:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UVerticalBox* PlayersTeamA;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UVerticalBox* PlayersTeamB;
};
