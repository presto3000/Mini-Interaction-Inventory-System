// Copyright Epic Games, Inc. All Rights Reserved.


#include "InterInvSystemPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "InterInvSystem.h"
#include "Interaction/InteractionComponent.h"
#include "UI/InventoryWidget.h"
#include "UI/MainPlayerOverlayWidget.h"
#include "Widgets/Input/SVirtualJoystick.h"

void AInterInvSystemPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogInterInvSystem, Error, TEXT("Could not spawn mobile controls widget."));
		}
	}

	if (PlayerOverlayWidgetClass)
	{
		PlayerOverlayWidget = CreateWidget<UMainPlayerOverlayWidget>(this,PlayerOverlayWidgetClass);
		if (PlayerOverlayWidget)
		{
			PlayerOverlayWidget->AddToViewport();
			InventoryWidget = PlayerOverlayWidget->InventoryWidget;
		}
	}


	if (APawn* Pawn_ = GetPawn())
	{
		if (UInteractionComponent* Interaction =
			Pawn_->FindComponentByClass<UInteractionComponent>())
		{
			Interaction->OnFocusChanged.AddDynamic(this, &AInterInvSystemPlayerController::OnFocusChanged);
		}
	}
	
}

void AInterInvSystemPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void AInterInvSystemPlayerController::OnFocusChanged(AActor* Actor, FText Text)
{
	if (!PlayerOverlayWidget)
	{
		return;
	}
	
	if (Actor)
	{
		PlayerOverlayWidget->SetInteractionText(Text);
	}
	else
	{
		PlayerOverlayWidget->HideInteractionPrompt();
	}
}

void AInterInvSystemPlayerController::ToggleInventory()
{
	if(!InventoryWidget)
		return;

	bInventoryOpen = !bInventoryOpen;

	if(bInventoryOpen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(InputMode);

		bShowMouseCursor = true;

		InventoryWidget->RefreshInventory();
	}
	else
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

		SetInputMode(FInputModeGameOnly());

		bShowMouseCursor = false;
	}
}
