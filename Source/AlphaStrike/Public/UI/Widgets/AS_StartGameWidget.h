// AlphaStrike by Team #1. AlphaNova courses🤙

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AS_StartGameWidget.generated.h"

UCLASS()
class ALPHASTRIKE_API UAS_StartGameWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UCanvasPanel* Canvas;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* DisplayText;

public:
    void InitializeWidget(float DelayStartTimer);

private:
    float StartGameDelay;
    float Timer;

    bool bStartTimer = false;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
