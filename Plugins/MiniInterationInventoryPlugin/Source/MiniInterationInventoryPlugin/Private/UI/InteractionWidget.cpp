// presto3000 Chris Pawlowski All Rights reserved.


#include "UI/InteractionWidget.h"

#include "Components/TextBlock.h"

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HidePrompt();
}


void UInteractionWidget::SetInteractionText(const FText& Text)
{
	if (InteractionText)
	{
		InteractionText->SetText(Text);
		SetVisibility(ESlateVisibility::Visible);
	}
}


void UInteractionWidget::HidePrompt()
{
	SetVisibility(ESlateVisibility::Hidden);
}