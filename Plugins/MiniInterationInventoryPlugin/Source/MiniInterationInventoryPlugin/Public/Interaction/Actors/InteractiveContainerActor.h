// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Interaction/ToggleTarget.h"
#include "Persistence/SaveableActorBase.h"
#include "InteractiveContainerActor.generated.h"

UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API AInteractiveContainerActor : public ASaveableActorBase, public IInteractable, public IToggleTarget
{
	GENERATED_BODY()

public:
	AInteractiveContainerActor();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Container")
	bool bIsOpen = false;

	virtual bool CanInteract_Implementation(AActor* InInstigator) override;
	virtual FText GetInteractionText_Implementation(AActor* InInstigator) override;
	virtual void Interact_Implementation(AActor* InInstigator) override;
	virtual void SetToggleState_Implementation(bool bNewState) override;
	virtual void RestoreSaveState(const TArray<uint8>& Bytes) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
};

