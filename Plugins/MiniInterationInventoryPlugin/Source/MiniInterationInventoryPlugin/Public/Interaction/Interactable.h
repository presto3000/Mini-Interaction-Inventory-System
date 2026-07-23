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
class MINIINTERACTIONINVENTORYPLUGIN_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	FText GetInteractionText(AActor* InInstigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* InInstigator);
};
