// presto3000 Chris Pawlowski All Rights reserved.


#include "Items/ItemPickupActor.h"


#include "Components/StaticMeshComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Items/ItemDefinition.h"
#include "Subsystems/WorldStateSubsystem.h"
#include "Engine/AssetManager.h"

AItemPickupActor::AItemPickupActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    
}

void AItemPickupActor::BeginPlay()
{
    Super::BeginPlay();
    ApplyVisualsFromDefinition();
}

void AItemPickupActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
#if WITH_EDITOR
    if (GIsEditor)
    {
        FAssetData AssetData;

        if (UAssetManager::Get().GetPrimaryAssetData(ItemId, AssetData))
        {
            UItemDefinition* Definition = Cast<UItemDefinition>(AssetData.GetAsset());

            if (Definition)
            {
                MeshComponent->SetStaticMesh(Definition->PickupMesh.LoadSynchronous());
            }
        }
    }
#endif
}

void AItemPickupActor::ApplyVisualsFromDefinition()
{
    UAssetManager& AssetManager = UAssetManager::Get();
    const FSoftObjectPath DefinitionPath = AssetManager.GetPrimaryAssetPath(ItemId);
    UItemDefinition* Definition = Cast<UItemDefinition>(DefinitionPath.TryLoad());

    if (!Definition || Definition->PickupMesh.IsNull())
    {
        return;
    }

    TWeakObjectPtr<AItemPickupActor> WeakThis(this);
    AssetManager.GetStreamableManager().RequestAsyncLoad(
        Definition->PickupMesh.ToSoftObjectPath(),
        [WeakThis, Definition]()
        {
            if (AItemPickupActor* Self = WeakThis.Get())
            {
                Self->MeshComponent->SetStaticMesh(Definition->PickupMesh.Get());
            }
        }
    );
}

bool AItemPickupActor::CanInteract_Implementation(AActor* InInstigator)
{
    return InInstigator && InInstigator->FindComponentByClass<UInventoryComponent>() != nullptr;
}

FText AItemPickupActor::GetInteractionText_Implementation(AActor* InInstigator)
{
    UAssetManager& AssetManager = UAssetManager::Get();
    const FSoftObjectPath DefinitionPath = AssetManager.GetPrimaryAssetPath(ItemId);
    if (UItemDefinition* Definition = Cast<UItemDefinition>(DefinitionPath.TryLoad()))
    {
        return FText::Format(NSLOCTEXT("Interaction", "PickupFmt", "Pick up {0}"), Definition->DisplayName);
    }
    return NSLOCTEXT("Interaction", "PickupFallback", "Pick up item");
}

void AItemPickupActor::Interact_Implementation(AActor* InInstigator)
{
    UInventoryComponent* Inventory = InInstigator ? InInstigator->FindComponentByClass<UInventoryComponent>() : nullptr;
    if (!Inventory)
    {
        return;
    }

    int32 AmountAdded = 0;
    const EInventoryOpResult Result = Inventory->AddItem(ItemId, Quantity, AmountAdded);

    if (Result == EInventoryOpResult::Success)
    {
        if (UGameInstance* GI = GetGameInstance())
        {
            UWorldStateSubsystem* State = GI->GetSubsystem<UWorldStateSubsystem>();

            if (State)
            {
                State->MarkActorDestroyed(SaveId);
            }
        }
        Destroy();
    }
    else if (Result == EInventoryOpResult::Partial)
    {
        // Only part of the stack fit; leave the pickup in the world holding the rest.
        Quantity -= AmountAdded;
    }
}

void AItemPickupActor::RestoreSaveState(const TArray<uint8>& Bytes)
{
    Super::RestoreSaveState(Bytes);
}
