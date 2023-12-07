// AlphaStrike by Team #1. AlphaNova courses🤙

#include "AS_GameInstance.h"
#include "Sound/SoundClass.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"
#include "MoviePlayer.h"

void UAS_GameInstance::Init() 
{
    Super::Init();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAS_GameInstance::BeginLoadingScreen);
}

void UAS_GameInstance::BeginLoadingScreen(const FString& InMapName)
{
    if (IsRunningDedicatedServer()) return;

    FLoadingScreenAttributes Attributes;
    Attributes.bAutoCompleteWhenLoadingCompletes = false;
    Attributes.bMoviesAreSkippable = true;
    Attributes.MinimumLoadingScreenDisplayTime = 2.f;

    UUserWidget* LoadingScreen = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenClass);
    if (!LoadingScreen) return;

    Attributes.WidgetLoadingScreen = LoadingScreen->TakeWidget();

    IGameMoviePlayer* MoviePlayer = GetMoviePlayer();
    if (MoviePlayer)
    {
        MoviePlayer->SetupLoadingScreen(Attributes);
    }
}

void UAS_GameInstance::SetMasterSoundVolume(float Volume)
{
    if (!MasterSoundClass) return;

    Volume = FMath::Clamp(Volume, 0.f, 1.f);
    MasterSoundClass->Properties.Volume = Volume;
}

float UAS_GameInstance::GetMasterSoundVolume() const
{
    if (!MasterSoundClass) return 0.0f;

    return MasterSoundClass->Properties.Volume;
}
