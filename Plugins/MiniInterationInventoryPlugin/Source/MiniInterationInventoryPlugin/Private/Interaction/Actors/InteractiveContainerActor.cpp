// presto3000 Chris Pawlowski All Rights reserved.


#include "Interaction/Actors/InteractiveContainerActor.h"

#include "Interaction/ToggleTarget.h"


AInteractiveContainerActor::AInteractiveContainerActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

bool AInteractiveContainerActor::CanInteract_Implementation(AActor* InInstigator)
{
	return true;
}

FText AInteractiveContainerActor::GetInteractionText_Implementation(AActor* InInstigator)
{
	return bIsOpen
		? NSLOCTEXT("Interaction", "ContainerClose", "Close container")
		: NSLOCTEXT("Interaction", "ContainerOpen", "Open container");
}

void AInteractiveContainerActor::Interact_Implementation(AActor* InInstigator)
{
	//SetToggleState(!bIsOpen);
	IToggleTarget::Execute_SetToggleState(this, !bIsOpen);
}

void AInteractiveContainerActor::SetToggleState_Implementation(bool bNewState)
{
	bIsOpen = bNewState;
	
}

void AInteractiveContainerActor::RestoreSaveState(const TArray<uint8>& Bytes)
{
	Super::RestoreSaveState(Bytes);
	IToggleTarget::Execute_SetToggleState(this, bIsOpen);
}

