// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "AS_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AAS_Character;
class AAS_HUD;

UCLASS()
class ALPHASTRIKE_API AAS_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void Pause();
    void UnPause();
    void ExitToMenu();

    void SetHealthBarPercent(float Percent);
    void SetShieldBarPercent(float Percent);

    void SetAmmoInfo(FText NewAmmoInfo);

    void CreateStartGameWidget(float StartGameDelay);

protected:
    void BeginPlay() override;

    void OnPossess(APawn* InPawn) override;

    void SetupInputComponent() override;

protected:
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void ShowCrosshair(const FInputActionValue& Value);

    void Jump();
    void StopJump();

    void Crouch();
    void StopCrouch();

    void Aim();
    void StopAim();

    void Shoot();
    void Reload();

    void ShowStatsTable();
    void HideStatsTable();

private:
    UFUNCTION(Client, Reliable)
    void Client_CreateStartGameWidget(float StartGameDelay);

private:
    UPROPERTY(Replicated)
    AAS_Character* PlayerCharacter;

    UPROPERTY()
    AAS_HUD* AS_HUD;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* CrouchAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* AimAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* ReloadAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* ShowStatsTableAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* PauseAction;

private:
    void SetInputModeGameOnly();
    void SetInputModeUIOnly();
};
