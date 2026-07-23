// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InteractionPersistenceLibrary.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UInteractionPersistenceLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Save", meta = (WorldContext = "WorldContextObject"))
	static bool SaveWorldAndInventory(UObject* WorldContextObject, UInventoryComponent* Inventory, const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save", meta = (WorldContext = "WorldContextObject"))
	static bool LoadWorldAndInventory(UObject* WorldContextObject, UInventoryComponent* Inventory, const FString& SlotName);
};

