// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlayWidget.generated.h"

class UInventoryWidget;
class UInteractionWidget;
class UButton;
/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UPlayerOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;
	void SetInteractionText(FText Text);
	void HideInteractionPrompt();

	UPROPERTY(meta=(BindWidget))
	UInventoryWidget* InventoryWidget;

protected:
	
	UFUNCTION()
	virtual void OnSaveClicked();

	UFUNCTION()
	virtual void OnLoadClicked();
	
private:

	UPROPERTY(meta=(BindWidget))
	UInteractionWidget* InteractionWidget;

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Save;

	UPROPERTY(meta=(BindWidget))
	UButton* Btn_Load;

};

