// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InterInvSystemPlayerController.generated.h"

class UMainPlayerOverlayWidget;
class UInventoryWidget;
class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class AInterInvSystemPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void ToggleInventory();
	
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;
	
	UFUNCTION()
	void OnFocusChanged(AActor* Actor, FText Text);
	
	UPROPERTY()
	UMainPlayerOverlayWidget* PlayerOverlayWidget;
	
	UPROPERTY()
	UInventoryWidget* InventoryWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainPlayerOverlayWidget> PlayerOverlayWidgetClass;
	
	bool bInventoryOpen = false;
};
