// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UInventoryWidget;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(FPrimaryAssetId InID,int32 InQuantity,UInventoryWidget* Parent);
	
	UFUNCTION()
	void SetSelected(bool bInSelected);

	UFUNCTION(BlueprintPure)
	FPrimaryAssetId GetItemId() const {	return ItemID; }
	
protected:
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	UButton* ItemButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemText;
	
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	
	UPROPERTY()
	UInventoryWidget* InventoryParent;
	
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	FPrimaryAssetId ItemID;
	
	int32 Quantity = 0;

	UFUNCTION()
	void Clicked();
	
	void RefreshVisuals();
	void LoadIcon();
	
	UPROPERTY(BlueprintReadOnly, Category="Inventory")
	bool bSelected = false;

	void UpdateSelectionVisual();

	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void BP_OnSelectionChanged(bool bIsSelected);

};
