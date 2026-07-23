// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SetInteractionText(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void HidePrompt();

protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* InteractionText;
};

