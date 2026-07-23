// presto3000 Chris Pawlowski All Rights reserved.


#include "Inventory/InventoryComponent.h"

#include "Items/ItemDefinition.h"
#include "Items/ItemUseEffect.h"
#include "Engine/AssetManager.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

UItemDefinition* UInventoryComponent::ResolveItemDefinition(const FPrimaryAssetId& ItemId)
{
    UAssetManager& AssetManager = UAssetManager::Get();
    const FSoftObjectPath ItemPath = AssetManager.GetPrimaryAssetPath(ItemId);
    if (!ItemPath.IsValid())
    {
        return nullptr;
    }
    return Cast<UItemDefinition>(ItemPath.TryLoad());
}

EInventoryOpResult UInventoryComponent::AddItem(FPrimaryAssetId ItemId, int32 Quantity, int32& OutAmountAdded)
{
    OutAmountAdded = 0;

    if (Quantity <= 0)
    {
        return EInventoryOpResult::Failed_InvalidItem;
    }

    UItemDefinition* Definition = ResolveItemDefinition(ItemId);
    if (!Definition)
    {
        UE_LOG(LogTemp, Warning, TEXT("Inventory: unknown or missing item definition for %s"), *ItemId.ToString());
        return EInventoryOpResult::Failed_InvalidItem;
    }

    int32 Remaining = Quantity;

    if (Definition->bIsStackable)
    {
        for (FInventoryEntry& Entry : Entries)
        {
            if (Entry.ItemId == ItemId && Entry.Quantity < Definition->MaxStackSize)
            {
                const int32 SpaceInStack = Definition->MaxStackSize - Entry.Quantity;
                const int32 ToAdd = FMath::Min(SpaceInStack, Remaining);
                Entry.Quantity += ToAdd;
                Remaining -= ToAdd;
                OutAmountAdded += ToAdd;

                if (Remaining <= 0)
                {
                    break;
                }
            }
        }
    }

    while (Remaining > 0 && Entries.Num() < MaxSlots)
    {
        FInventoryEntry NewEntry;
        NewEntry.ItemId = ItemId;
        NewEntry.Quantity = Definition->bIsStackable ? FMath::Min(Remaining, Definition->MaxStackSize) : 1;
        Entries.Add(NewEntry);
        Remaining -= NewEntry.Quantity;
        OutAmountAdded += NewEntry.Quantity;

        if (!Definition->bIsStackable)
        {
            // Non-stackable items always occupy exactly one slot each; loop
            // again if more than one copy was requested and slots remain.
            continue;
        }
    }

    if (OutAmountAdded > 0)
    {
        OnInventoryChanged.Broadcast();
    }

    if (Remaining > 0)
    {
        return OutAmountAdded > 0 ? EInventoryOpResult::Partial : EInventoryOpResult::Failed_NoSpace;
    }

    return EInventoryOpResult::Success;
}

EInventoryOpResult UInventoryComponent::RemoveItem(FPrimaryAssetId ItemId, int32 Quantity)
{
    if (Quantity <= 0)
    {
        return EInventoryOpResult::Failed_InvalidItem;
    }

    int32 Remaining = Quantity;
    bool bChanged = false;

    for (int32 Index = Entries.Num() - 1; Index >= 0 && Remaining > 0; --Index)
    {
        FInventoryEntry& Entry = Entries[Index];
        if (Entry.ItemId != ItemId)
        {
            continue;
        }

        const int32 ToRemove = FMath::Min(Entry.Quantity, Remaining);
        Entry.Quantity -= ToRemove;
        Remaining -= ToRemove;
        bChanged = true;

        if (Entry.Quantity <= 0)
        {
            Entries.RemoveAt(Index);
        }
    }

    if (bChanged)
    {
        OnInventoryChanged.Broadcast();
    }

    return Remaining > 0 ? EInventoryOpResult::Failed_NotFound : EInventoryOpResult::Success;
}

EInventoryOpResult UInventoryComponent::UseItem(FPrimaryAssetId ItemId)
{
    if (GetQuantityOf(ItemId) <= 0)
    {
        return EInventoryOpResult::Failed_NotFound;
    }

    UItemDefinition* Definition = ResolveItemDefinition(ItemId);
    if (!Definition)
    {
        return EInventoryOpResult::Failed_InvalidItem;
    }

    AActor* Owner = GetOwner();
    bool bShouldConsume = false;
    if (Definition->UseEffect)
    {
        bShouldConsume = Definition->UseEffect->OnUse(Owner);
    }
    
    if (bShouldConsume)
    {
        RemoveItem(ItemId, 1);
    }

    OnItemUsed.Broadcast(ItemId);
    return EInventoryOpResult::Success;
}

int32 UInventoryComponent::GetQuantityOf(FPrimaryAssetId ItemId) const
{
    int32 Total = 0;
    for (const FInventoryEntry& Entry : Entries)
    {
        if (Entry.ItemId == ItemId)
        {
            Total += Entry.Quantity;
        }
    }
    return Total;
}

void UInventoryComponent::RequestLoadIcon(FPrimaryAssetId ItemId, TFunction<void(UTexture2D*)> Callback)
{
    UItemDefinition* Definition = ResolveItemDefinition(ItemId);
    if (!Definition || Definition->Icon.IsNull())
    {
        Callback(nullptr);
        return;
    }

    if (Definition->Icon.IsValid())
    {
        Callback(Definition->Icon.Get());
        return;
    }

    UAssetManager& AssetManager = UAssetManager::Get();
    ActiveIconLoad = AssetManager.GetStreamableManager().RequestAsyncLoad(
        Definition->Icon.ToSoftObjectPath(),
        [Definition, Callback]()
        {
            Callback(Definition->Icon.Get());
        }
    );
}

void UInventoryComponent::LoadSaveData(const TArray<FInventoryEntry>& SavedEntries)
{
    Entries = SavedEntries;
    OnInventoryChanged.Broadcast();
}

// --- Debug tool -------------------------------------------------------
// Console command: "Inventory.Dump" prints the local player's inventory.
static FAutoConsoleCommandWithWorld GDumpInventoryCommand(
    TEXT("Inventory.Dump"),
    TEXT("Prints the inventory contents of the local player's pawn."),
    FConsoleCommandWithWorldDelegate::CreateStatic(
        [](UWorld* World)
        {
            if (!World)
            {
                return;
            }

            APlayerController* PC = World->GetFirstPlayerController();
            APawn* Pawn = PC ? PC->GetPawn() : nullptr;
            UInventoryComponent* Inventory = Pawn ? Pawn->FindComponentByClass<UInventoryComponent>() : nullptr;

            if (!Inventory)
            {
                UE_LOG(LogTemp, Warning, TEXT("Inventory.Dump: no UInventoryComponent found on the player's pawn."));
                return;
            }

            UE_LOG(LogTemp, Log, TEXT("=== Inventory Dump (%d/%d slots) ==="), Inventory->GetEntries().Num(), Inventory->MaxSlots);
            for (const FInventoryEntry& Entry : Inventory->GetEntries())
            {
                UE_LOG(LogTemp, Log, TEXT("  %s x%d"), *Entry.ItemId.ToString(), Entry.Quantity);
            }
        }
    )
);
