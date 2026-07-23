// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "InventoryComponent.generated.h"


class UItemDefinition;

// Runtime record only: which item, how many. The item's actual data
// (name, stack size, icon...) is resolved on demand from UItemDefinition,

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FPrimaryAssetId ItemId;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Quantity = 0;
};

UENUM(BlueprintType)
enum class EInventoryOpResult : uint8
{
	Success,
	Partial,
	Failed_NoSpace,
	Failed_InvalidItem,
	Failed_NotFound
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemUsed, FPrimaryAssetId, ItemId);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIINTERACTIONINVENTORYPLUGIN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	static UItemDefinition* ResolveItemDefinition(const FPrimaryAssetId& ItemId);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots = 20;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemUsed OnItemUsed;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	EInventoryOpResult AddItem(FPrimaryAssetId ItemId, int32 Quantity, int32& OutAmountAdded);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	EInventoryOpResult RemoveItem(FPrimaryAssetId ItemId, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	EInventoryOpResult UseItem(FPrimaryAssetId ItemId);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<FInventoryEntry>& GetEntries() const { return Entries; }

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetQuantityOf(FPrimaryAssetId ItemId) const;

	// Async icon fetch so UI never has to block the game thread on a load.
	void RequestLoadIcon(FPrimaryAssetId ItemId, TFunction<void(UTexture2D*)> Callback);

	TArray<FInventoryEntry> GetSaveData() const { return Entries; }
	void LoadSaveData(const TArray<FInventoryEntry>& SavedEntries);

private:
	UPROPERTY()
	TArray<FInventoryEntry> Entries;

	TSharedPtr<FStreamableHandle> ActiveIconLoad;
};