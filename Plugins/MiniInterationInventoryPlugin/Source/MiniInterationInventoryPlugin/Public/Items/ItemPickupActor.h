// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/Interactable.h"
#include "Persistence/SaveableActorBase.h"
#include "ItemPickupActor.generated.h"

UCLASS()
class MINIINTERACTIONINVENTORYPLUGIN_API AItemPickupActor : public ASaveableActorBase, public IInteractable
{
	GENERATED_BODY()

public:
	AItemPickupActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FPrimaryAssetId ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
	int32 Quantity = 1;

	virtual bool CanInteract_Implementation(AActor* InInstigator) override;
	virtual FText GetInteractionText_Implementation(AActor* InInstigator) override;
	virtual void Interact_Implementation(AActor* InInstigator) override;
	virtual void RestoreSaveState(const TArray<uint8>& Bytes) override;


protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

private:
	void ApplyVisualsFromDefinition();
};
