// AlphaStrike by Team #1. AlphaNova courses🤙

#include "UI/Widgets/AS_MenuWidget.h"
#include "Components/EditableText.h"
#include "Components/VerticalBox.h"

void UAS_MenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}


void UAS_MenuWidget::SetNameText(FText Name)
{
    if (NameBox)
    {
        NameBox->SetText(Name);
    }
}

FText UAS_MenuWidget::GetNameText()
{
    return NameBox->GetText();
}
