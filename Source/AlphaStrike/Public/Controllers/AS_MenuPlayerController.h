// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AS_MenuPlayerController.generated.h"

UCLASS()
class ALPHASTRIKE_API AAS_MenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void CreateTeamDeathmatchGame();
    void CreateDeathmatchGame();
    void CreateDeathmatchHostGame();
    void JoinGame(const FText& IP_Address);

protected:
    virtual void BeginPlay() override;
};
