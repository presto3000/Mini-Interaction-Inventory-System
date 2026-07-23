// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveableActorBase.generated.h"

// Common base for any map actor whose state should survive a save/load.
// State itself lives in ordinary UPROPERTY(SaveGame) members on the derived
// class (bIsOpen, bIsActive, etc.) - this base captures/restores them
// generically via reflection.

UCLASS()
class MINIINTERATIONINVENTORYPLUGIN_API ASaveableActorBase : public AActor
{
	GENERATED_BODY()

public:
	ASaveableActorBase();

	UPROPERTY(SaveGame, EditAnywhere, Category = "Save")
	FGuid SaveId;

	TArray<uint8> CaptureSaveState() const;
	virtual void RestoreSaveState(const TArray<uint8>& Bytes);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
};
