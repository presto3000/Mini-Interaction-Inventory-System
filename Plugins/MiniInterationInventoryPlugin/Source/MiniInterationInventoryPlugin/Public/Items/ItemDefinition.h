// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable,
	Equipment,
	KeyItem
};

/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemType ItemType = EItemType::Consumable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	bool bIsStackable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1", EditCondition = "bIsStackable"))
	int32 MaxStackSize = 99;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Visual")
	TSoftObjectPtr<UStaticMesh> PickupMesh;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(TEXT("Item"), GetFName());
	}

	// Left empty, using the item simply does nothing but still
	// reports success and lets UInventoryComponent::UseItem's normal flow
	// (consumption rules, OnItemUsed broadcast) run.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Item")
	TObjectPtr<class UItemUseEffect> UseEffect;
};
