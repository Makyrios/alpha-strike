// AlphaStrike by Team #1. AlphaNova courses🤙

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
}

void AAS_PlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    if (!InPawn) return;

    AHUD* hud = GetHUD();

    PlayerCharacter = Cast<AAS_Character>(InPawn);
}

void AAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Clear out existing mapping, and add our mapping
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    // Get the EnhancedInputComponent
    if (UEnhancedInputComponent* UEI = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind the actions
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
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    const FVector CrosshairStart = PlayerCharacter->GetCombatComponent()->GetStartMuzzlePoint();
    const FVector CrosshairEnd = PlayerCharacter->GetCombatComponent()->GetEndMuzzlePoint();
    PlayerCharacter->CrosshairActivate(CrosshairStart, CrosshairEnd);
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

    if (PlayerCharacter->GetCombatComponent()->GetWeaponInventory().Num() > 1)
    {
        StopAnimMontages();
        PlayerCharacter->GetCombatComponent()->ScrollWeaponUp();
        PlayerCharacter->UpdateHUDAmmoInfo();
    }
}

void AAS_PlayerController::ScrollWeaponDown()
{
    if (!PlayerCharacter || !PlayerCharacter->GetCombatComponent()) return;

    if (PlayerCharacter->GetCombatComponent()->GetWeaponInventory().Num() > 1)
    {
        StopAnimMontages();
        PlayerCharacter->GetCombatComponent()->ScrollWeaponDown();
        PlayerCharacter->UpdateHUDAmmoInfo();
    }
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

void AAS_PlayerController::StopAnimMontages()
{
    if (PlayerCharacter && PlayerCharacter->GetMesh() && PlayerCharacter->GetMesh()->GetAnimInstance())
    {
        PlayerCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(0.3);
    }
}

void AAS_PlayerController::Client_CreateStartGameWidget_Implementation(float StartGameDelay)
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowStartGameWidget(StartGameDelay);
}

void AAS_PlayerController::CreateStartGameWidget(float StartGameDelay)
{
    if (!IsLocalController())
    {
        Client_CreateStartGameWidget(StartGameDelay);
        return;
    }

    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowStartGameWidget(StartGameDelay);
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

void AAS_PlayerController::Client_HandleLose_Implementation()
{
    AS_HUD = (!AS_HUD) ? GetHUD<AAS_HUD>() : AS_HUD;
    if (!AS_HUD) return;

    AS_HUD->ShowLoseWidget();
}
