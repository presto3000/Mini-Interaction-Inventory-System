// presto3000 Chris Pawlowski All Rights reserved.


#include "MiniInterationInventoryPlugin/Public/Persistence/InteractionPersistenceLibrary.h"
#include "MiniInterationInventoryPlugin/Public/Persistence/InventorySaveGame.h"
#include "Inventory/InventoryComponent.h"
#include "MiniInterationInventoryPlugin/Public/Persistence/SaveableActorBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Subsystems/WorldStateSubsystem.h"

bool UInteractionPersistenceLibrary::SaveWorldAndInventory(UObject* WorldContextObject, UInventoryComponent* Inventory, const FString& SlotName)
{
    UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
    if (!World)
    {
        return false;
    }

    UInventorySaveGame* SaveGameObject = Cast<UInventorySaveGame>(UGameplayStatics::CreateSaveGameObject(UInventorySaveGame::StaticClass()));
    if (!SaveGameObject)
    {
        return false;
    }

    SaveGameObject->SlotName = SlotName;

    if (Inventory)
    {
        SaveGameObject->InventoryEntries = Inventory->GetSaveData();
    }

    for (TActorIterator<ASaveableActorBase> It(World); It; ++It)
    {
        ASaveableActorBase* SaveableActor = *It;
        if (SaveableActor && SaveableActor->SaveId.IsValid())
        {
            FActorSaveData Data;
            Data.Bytes = SaveableActor->CaptureSaveState();

            SaveGameObject->ActorStates.Add(SaveableActor->SaveId, Data);
        }
    }


    // Save destroyed/collected actors
    if (UGameInstance* GI = World->GetGameInstance())
    {
        UWorldStateSubsystem* State = GI->GetSubsystem<UWorldStateSubsystem>();
        if (State)
        {
            State->GetWorldState(SaveGameObject->WorldStates);
        }
    }
    
    return UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

bool UInteractionPersistenceLibrary::LoadWorldAndInventory(UObject* WorldContextObject, UInventoryComponent* Inventory, const FString& SlotName)
{
    UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
    if (!World || !UGameplayStatics::DoesSaveGameExist(SlotName, 0))
    {
        return false;
    }

    UInventorySaveGame* SaveGameObject = Cast<UInventorySaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
    if (!SaveGameObject)
    {
        return false;
    }

    if (UGameInstance* GI = World->GetGameInstance())
    {
        UWorldStateSubsystem* State = GI->GetSubsystem<UWorldStateSubsystem>();

        if (State)
        {
            State->LoadWorldState(SaveGameObject->WorldStates);
            UE_LOG(LogTemp, Warning, TEXT("Destroyed count loaded: %d"), SaveGameObject->WorldStates.Num());
        }
        
        if (Inventory)
        {
            Inventory->LoadSaveData(SaveGameObject->InventoryEntries);
        }

        for (TActorIterator<ASaveableActorBase> It(World); It; ++It)
        {
            ASaveableActorBase* SaveableActor = *It;
            if (!SaveableActor)
            {
                continue;
            }

            if (const FActorSaveData* Data = SaveGameObject->ActorStates.Find(SaveableActor->SaveId))
            {
                SaveableActor->RestoreSaveState(Data->Bytes);
            }

            if (State->IsActorDestroyed(SaveableActor->SaveId))
            {
                UE_LOG(LogTemp, Warning, TEXT("Destroying actor on load: %s"), *SaveableActor->GetName());
                SaveableActor->Destroy();
            }
            // Actor present now but missing from the save (didn't exist back then)
            // just keeps its defaults; actor missing now but present in the save
            // is silently skipped. Both are safe no-ops.
        }
    }



    return true;
}