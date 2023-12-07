// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AS_Types.h"
#include "AS_MenuHUD.generated.h"

class UAS_MenuWidget;
class AAS_MenuPlayerController;

UCLASS()
class ALPHASTRIKE_API AAS_MenuHUD : public AHUD
{
    GENERATED_BODY()

public:
    AAS_MenuHUD();
    void AddMenuWidget();

    void HandleMenuAction();

public:
    FORCEINLINE UAS_MenuWidget* GetMenuWidget() const { return MenuWidget; }

private:
    UPROPERTY(EditDefaultsOnly, Category = "AS|UI")
    TSubclassOf<UUserWidget> MenuWidgetClass;

    UPROPERTY()
    UAS_MenuWidget* MenuWidget;

    UPROPERTY()
    AAS_MenuPlayerController* AS_PlayerController;

    EMenuAction MenuAction = EMenuAction::EMA_MAX;

private:
    UFUNCTION()
    void OnSingleDeathmatchButtonClicked();

    UFUNCTION()
    void OnTeamDeathmatchButtonClicked();

    UFUNCTION()
    void OnHostButtonClicked();

    UFUNCTION()
    void OnJoinButtonClicked();

    UFUNCTION()
    void OnApplyNameButtonClicked();

    AAS_MenuPlayerController* GetPlayerController();
};
