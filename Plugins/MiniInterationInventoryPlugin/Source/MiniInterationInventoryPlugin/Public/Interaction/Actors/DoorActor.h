// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/ToggleTarget.h"
#include "Persistence/SaveableActorBase.h"
#include "DoorActor.generated.h"

UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API ADoorActor : public ASaveableActorBase, public IToggleTarget
{
	GENERATED_BODY()

public:
	ADoorActor();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	bool bIsOpen = false;

	virtual void SetToggleState_Implementation(bool bNewState) override;
	virtual void RestoreSaveState(const TArray<uint8>& Bytes) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
};