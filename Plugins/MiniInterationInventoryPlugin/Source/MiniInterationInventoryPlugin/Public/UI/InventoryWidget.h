// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryItemWidget;
class UButton;
class UVerticalBox;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	// =============================
	// Inventory
	// =============================
	
	UPROPERTY()
	UInventoryComponent* Inventory;
	UPROPERTY()
	FPrimaryAssetId SelectedItem;
	
protected:

	// =============================
	// UMG widgets
	// =============================
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* InventoryList;
	UPROPERTY(meta=(BindWidget))
	UButton* UseButton;
	UPROPERTY(meta=(BindWidget))
	UButton* RemoveButton;
	
	// =============================
	// Button callbacks
	// =============================
	
	UFUNCTION()
	void OnUseClicked();
	UFUNCTION()
	void OnRemoveClicked();
	UFUNCTION()
	void InventoryChanged();


	// Blueprint widget used for each inventory entry
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

	UPROPERTY()
	TArray<UInventoryItemWidget*> ItemWidgets;

public:
	
	// Called when player clicks item slot
	UFUNCTION(BlueprintCallable)
	void SelectItem(FPrimaryAssetId ItemId);
	
	// Rebuild list
	UFUNCTION(BlueprintCallable)
	void RefreshInventory();

};
