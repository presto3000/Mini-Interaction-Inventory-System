// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIINTERATIONINVENTORYPLUGIN_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	FText GetInteractionText(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* InInstigator);
};
