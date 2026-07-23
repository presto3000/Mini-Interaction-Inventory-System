// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Persistence/InventorySaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldStateSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API UWorldStateSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	void MarkActorDestroyed(FGuid ActorId);
	bool IsActorDestroyed(FGuid ActorId) const;
	void LoadWorldState(const TArray<FWorldStateEntry>& SavedStates);
	void GetWorldState(TArray<FWorldStateEntry>& OutStates) const;

private:

	UPROPERTY()
	TSet<FGuid> DestroyedActors;
};

