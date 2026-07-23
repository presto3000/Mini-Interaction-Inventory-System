// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Inventory/InventoryComponent.h"
#include "InventorySaveGame.generated.h"

USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> Bytes;
};

USTRUCT(BlueprintType)
struct FWorldStateEntry
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FGuid ActorId;

	UPROPERTY()
	bool bDestroyed = false;
};

/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UInventorySaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FInventoryEntry> InventoryEntries;

	// SaveId -> serialized SaveGame properties for map actors
	// (containers, switches, doors, ...). No per-class handling required.
	UPROPERTY()
	TMap<FGuid, FActorSaveData> ActorStates;

	// Destroyed / collected actors
	UPROPERTY()
	TArray<FWorldStateEntry> WorldStates;

	UPROPERTY()
	FString SlotName = TEXT("DefaultSlot");
};
