// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Persistence/SaveableActorBase.h"
#include "InteractiveSwitchActor.generated.h"

UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API AInteractiveSwitchActor : public ASaveableActorBase, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractiveSwitchActor();

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadOnly, Category = "Switch")
	bool bIsActive = false;

	// Any actor implementing IToggleTarget that should react when this flips.
	UPROPERTY(EditInstanceOnly, Category = "Switch")
	TArray<TObjectPtr<AActor>> LinkedActors;

	virtual bool CanInteract_Implementation(AActor* InInstigator) override;
	virtual FText GetInteractionText_Implementation(AActor* InInstigator) override;
	virtual void Interact_Implementation(AActor* InInstigator) override;
	virtual void RestoreSaveState(const TArray<uint8>& Bytes) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;
};
