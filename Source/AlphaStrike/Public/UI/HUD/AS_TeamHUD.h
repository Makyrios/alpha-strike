// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/AS_HUD.h"
#include "AS_TeamHUD.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_TeamHUD : public AAS_HUD
{
    GENERATED_BODY()

public:
    void SetTeamAScore(int32 NewScore) override;
    void SetTeamBScore(int32 NewScore) override;
};
