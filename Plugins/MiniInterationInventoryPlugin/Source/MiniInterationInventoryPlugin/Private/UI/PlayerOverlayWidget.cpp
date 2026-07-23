// presto3000 Chris Pawlowski All Rights reserved.


#include "UI/PlayerOverlayWidget.h"

#include "Components/Button.h"
#include "UI/InteractionWidget.h"

void UPlayerOverlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Btn_Save)
	{
		Btn_Save->OnClicked.AddDynamic(this, &UPlayerOverlayWidget::OnSaveClicked);
	}

	if (Btn_Load)
	{
		Btn_Load->OnClicked.AddDynamic(this, &UPlayerOverlayWidget::OnLoadClicked);
	}
}

void UPlayerOverlayWidget::SetInteractionText(FText Text)
{
	if (InteractionWidget)
	{
		InteractionWidget->SetInteractionText(Text);
	}
}

void UPlayerOverlayWidget::HideInteractionPrompt()
{
	if (InteractionWidget)
	{
		InteractionWidget->HidePrompt();
	}
}

void UPlayerOverlayWidget::OnSaveClicked()
{
	// override by child

}

void UPlayerOverlayWidget::OnLoadClicked()
{
	// override by child

}
