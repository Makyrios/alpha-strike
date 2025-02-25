﻿// AlphaStrike by Team #1. AlphaNova courses🤙

#include "Controllers/AS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Characters/AS_Character.h"
#include "Components/AS_CombatComponent.h"
#include "Net/UnrealNetwork.h"
#include "UI/HUD/AS_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "AS_GameInstance.h"
#include "Weapons/AS_BaseWeapon.h"
#include "GameStates/AS_TeamDeathmatchGameState.h"
#include "PlayerStates/AS_TeamDeathmatchPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"

void AAS_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAS_PlayerController, PlayerCharacter);
}

void AAS_PlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameOnly InputModeData;
    SetInputMode(InputModeData);
    SetShowMouseCursor(false);

    if (GameSound && GetWorld() && IsLocalController())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), GameSound);
    }
}

void AAS_PlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    PlayerCharacter = Cast<AAS_Character>(InPawn);

    if (IsLocalController())
    {
        UpdateInventoryInfo();
    }
}

void AAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    if (UEnhancedInputComponent* UEI = Cast<UEnhancedInputComponent>(InputComponent))
    {
        UEI->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAS_PlayerController::Look);
        UEI->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAS_PlayerController::Move);
        UEI->BindAction(JumpAction, ETriggerEvent::Started, this, &AAS_PlayerController::Jump);
        UEI->BindAction(JumpAction, ETriggerEvent::Completed, this, &AAS_PlayerController::StopJump);
        UEI->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAS_PlayerController::Crouch);
        UEI->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AAS_PlayerController::StopCrouch);
        UEI->BindAction(AimAction, ETriggerEvent::Started, this, &AAS_PlayerController::Aim);
        UEI->BindAction(AimAction, ETriggerEvent::Completed, this, &AAS_PlayerController::StopAim);
        UEI->BindAction(AimAction, ETriggerEvent::Triggered, this, &AAS_PlayerController::ShowCrosshair);
        UEI->BindAction(FireAction, ETriggerEvent::Started, this, &AAS_PlayerController::Shoot);
        UEI->BindAction(FireAction, ETriggerEvent::Completed, this, &AAS_PlayerController::StopShoot);
        UEI->BindAction(ReloadAction, ETriggerEvent::Started, this, &AAS_PlayerController::Reload);
        UEI->BindAction(ShowStatsTableAction, ETriggerEvent::Started, this, &AAS_PlayerController::ShowStatsTable);
        UEI->BindAction(ShowStatsTableAction, ETriggerEvent::Completed, this, &AAS_PlayerController::HideStatsTable);
        UEI->BindAction(PauseAction, ETriggerEvent::Started, this, &AAS_PlayerController::Pause);
        UEI->BindAction(ScrollWeaponUpAction, ETriggerEvent::Started, this, &AAS_PlayerController::ScrollWeaponUp);
        UEI->BindAction(ScrollWeaponDownAction, ETriggerEvent::Started, this, &AAS_PlayerController::ScrollWeaponDown);
    }
}

void AAS_PlayerController::Move(const FInputActionValue& Value)
{
    if (!PlayerCharacter) return;

    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    // find out which way is forward
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    // get forward vector
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    // get right vector
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // add movement
    PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
    PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void AAS_PlayerController::Look(const FInputActionValue& Value)
{
    if (!PlayerCharacter) return;

    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    // add yaw and pitch input to controller
    PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
    PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void AAS_PlayerController::ShowCrosshair(const FInputActionValue& Value)
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent() || !PlayerCharacter->GetCombatComponent()->GetEquippedWeapon()) return;

    if (PlayerCharacter->GetCombatComponent()->GetEquippedWeapon()->CanFire())
    {
        PlayerCharacter->CrosshairActivate();
    }
    else
    {
        PlayerCharacter->CrosshairDeactivate();
    }
}

void AAS_PlayerController::Jump()
{
    if (!PlayerCharacter || !GetWorld() || GetWorld()->GetTimerManager().IsTimerActive(JumpTimer)) return;

    GetWorld()->GetTimerManager().SetTimer(JumpTimer, JumpDelay, false);

    PlayerCharacter->Jump();
}

void AAS_PlayerController::StopJump()
{
    if (!PlayerCharacter) return;

    PlayerCharacter->StopJumping();
}

void AAS_PlayerController::Crouch()
{
    if (!PlayerCharacter) return;

    PlayerCharacter->Crouch();
}

void AAS_PlayerController::StopCrouch()
{
    if (!PlayerCharacter) return;

    PlayerCharacter->UnCrouch();
}

void AAS_PlayerController::Aim()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->GetCombatComponent()->Aim();
}

void AAS_PlayerController::StopAim()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->CrosshairDeactivate();
    PlayerCharacter->GetCombatComponent()->StopAim();
}

void AAS_PlayerController::Shoot()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->GetCombatComponent()->Fire();
}

void AAS_PlayerController::StopShoot()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->GetCombatComponent()->StopFire();
}

void AAS_PlayerController::Reload()
{
    if (!PlayerCharacter || !PlayerCharacter->GetEquippedWeapon()) return;

    PlayerCharacter->GetEquippedWeapon()->StartReload();
}

void AAS_PlayerController::ShowStatsTable()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowStatsTable();
}

void AAS_PlayerController::HideStatsTable()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->HideStatsTable();
}

void AAS_PlayerController::SetHealthBarPercent(float Percent)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetHealthBarPercent(Percent);
}

void AAS_PlayerController::SetShieldBarPercent(float Percent)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetShieldBarPercent(Percent);
}

void AAS_PlayerController::PlayDamageAnimation()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->PlayDamageAnimation();
}

void AAS_PlayerController::SetAmmoInfo(FText NewAmmoInfo)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetAmmoInfo(NewAmmoInfo);
}

void AAS_PlayerController::SetScoreGoal(int32 ScoreGoal)
{
    UpdateScoreGoal(ScoreGoal);
    Client_SetScoreGoal(ScoreGoal);
}

void AAS_PlayerController::SetTeamsScore()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;

    AGameStateBase* GameState = UGameplayStatics::GetGameState(this);
    if (!GameState || !AS_HUD) return;

    AAS_TeamDeathmatchGameState* TeamGameState = Cast<AAS_TeamDeathmatchGameState>(GameState);
    if (TeamGameState)
    {
        AS_HUD->SetTeamAScore(TeamGameState->GetTeamScore(ETeams::TEAM_A));
        AS_HUD->SetTeamBScore(TeamGameState->GetTeamScore(ETeams::TEAM_B));
    }
}

void AAS_PlayerController::ScrollWeaponUp()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->GetCombatComponent()->ScrollWeaponUp();
}

void AAS_PlayerController::ScrollWeaponDown()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    PlayerCharacter->GetCombatComponent()->ScrollWeaponDown();
}

void AAS_PlayerController::Client_SetTime_Implementation(const float& Time)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetTimeRemaining(Time);
}

void AAS_PlayerController::Pause()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->Pause(true);
    SetInputModeUIOnly();
}

void AAS_PlayerController::UnPause()
{
    SetInputModeGameOnly();
}

void AAS_PlayerController::ExitToMenu()
{
    const auto GameInstance = GetGameInstance<UAS_GameInstance>();
    if (!GameInstance) return;

    UGameplayStatics::OpenLevel(this, GameInstance->MenuMapName);
}

void AAS_PlayerController::SetInputModeGameOnly()
{
    FInputModeGameOnly InputModeGameOnly;
    SetInputMode(InputModeGameOnly);
    SetShowMouseCursor(false);
}

void AAS_PlayerController::SetInputModeUIOnly()
{
    FInputModeUIOnly InputModeUI;
    SetInputMode(InputModeUI);
    SetShowMouseCursor(true);
}

void AAS_PlayerController::Client_SetScoreGoal_Implementation(int32 ScoreGoal)
{
    UpdateScoreGoal(ScoreGoal);
}

void AAS_PlayerController::UpdateScoreGoal(int ScoreGoal)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetScoreGoal(ScoreGoal);
}

void AAS_PlayerController::Client_CreateStartGameWidget_Implementation(float StartGameDelay)
{
    ShowStartGameWidget(StartGameDelay);
}

void AAS_PlayerController::CreateStartGameWidget(float StartGameDelay)
{
    if (HasAuthority() && IsLocalController())
    {
        ShowStartGameWidget(StartGameDelay);
    }
    else
    {
        Client_CreateStartGameWidget(StartGameDelay);
    }
}

void AAS_PlayerController::ShowStartGameWidget(float StartGameDelay) 
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowStartGameWidget(StartGameDelay);
}

void AAS_PlayerController::SetHUDWidgetVisibility(ESlateVisibility InVisibility)
{
    if (!IsLocalController())
    {
        Client_SetHUDWidgetVisibility(InVisibility);
    }

    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetHUDWidgetVisibility(InVisibility);
}

void AAS_PlayerController::Client_SetHUDWidgetVisibility_Implementation(ESlateVisibility InVisibility)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->SetHUDWidgetVisibility(InVisibility);
}

void AAS_PlayerController::HandleWin()
{
    if (!IsLocalController())
    {
        Client_HandleWin();
        return;
    }

    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowWonWidget();
}

void AAS_PlayerController::Client_HandleWin_Implementation()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowWonWidget();
}

void AAS_PlayerController::HandleLose()
{
    if (!IsLocalController())
    {
        Client_HandleLose();
        return;
    }

    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowLoseWidget();
}

void AAS_PlayerController::SetTimeRemaining(float RemainingTimeInSeconds)
{
    if (HasAuthority() && IsLocalController())
    {
        AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
        if (!AS_HUD) return;

        AS_HUD->SetTimeRemaining(RemainingTimeInSeconds);
    }
    else
    {
        Client_SetTime(RemainingTimeInSeconds);
    }
}

void AAS_PlayerController::Client_HandleLose_Implementation()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowLoseWidget();
}

void AAS_PlayerController::UpdateInventoryInfo()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->UpdateInventoryInfo();
}
