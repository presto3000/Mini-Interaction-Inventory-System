// presto3000 Chris Pawlowski All Rights reserved.


#include "Subsystems/WorldStateSubsystem.h"
#include "Persistence/InventorySaveGame.h"

void UWorldStateSubsystem::MarkActorDestroyed(FGuid ActorId)
{
	DestroyedActors.Add(ActorId);
}


bool UWorldStateSubsystem::IsActorDestroyed(FGuid ActorId) const
{
	const bool bResult = DestroyedActors.Contains(ActorId);

	UE_LOG(LogTemp, Warning,
		TEXT("IsActorDestroyed Check: %s Result: %s TotalDestroyed: %d"),
		*ActorId.ToString(),
		bResult ? TEXT("TRUE") : TEXT("FALSE"),
		DestroyedActors.Num());

	return bResult;
}


void UWorldStateSubsystem::LoadWorldState(const TArray<FWorldStateEntry>& SavedStates)
{
	DestroyedActors.Empty();

	UE_LOG(LogTemp, Warning,TEXT("Loading World State. Saved Entries: %d"),	SavedStates.Num());
	
	for (const FWorldStateEntry& Entry : SavedStates)
	{
		UE_LOG(LogTemp, Warning,TEXT("WorldState Entry: ID=%s Destroyed=%s"), *Entry.ActorId.ToString(),
			Entry.bDestroyed ? TEXT("TRUE") : TEXT("FALSE"));
		
		if (Entry.bDestroyed)
		{
			DestroyedActors.Add(Entry.ActorId);
		}
	}
	
	UE_LOG(LogTemp, Warning,TEXT("World State Loaded. DestroyedActors Count: %d"),	DestroyedActors.Num());

	for (const FGuid& ID : DestroyedActors)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Destroyed Actor ID Stored: %s"),
			*ID.ToString());
	}
}


void UWorldStateSubsystem::GetWorldState(TArray<FWorldStateEntry>& OutStates) const
{
	OutStates.Empty();

	for (const FGuid& ID : DestroyedActors)
	{
		FWorldStateEntry Entry;

		Entry.ActorId = ID;
		Entry.bDestroyed = true;

		OutStates.Add(Entry);
	}
}