// presto3000 Chris Pawlowski All Rights reserved.


#include "Interaction/Actors/InteractiveSwitchActor.h"

#include "Interaction/ToggleTarget.h"


AInteractiveSwitchActor::AInteractiveSwitchActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

bool AInteractiveSwitchActor::CanInteract_Implementation(AActor* InInstigator)
{
	return true;
}

FText AInteractiveSwitchActor::GetInteractionText_Implementation(AActor* InInstigator)
{
	return NSLOCTEXT("Interaction", "SwitchUse", "Use switch");
}

void AInteractiveSwitchActor::Interact_Implementation(AActor* InInstigator)
{
	bIsActive = !bIsActive;

	for (AActor* Linked : LinkedActors)
	{
		if (Linked && Linked->Implements<UToggleTarget>())
		{
			IToggleTarget::Execute_SetToggleState(Linked, bIsActive);
		}
	}
}

void AInteractiveSwitchActor::RestoreSaveState(const TArray<uint8>& Bytes)
{
	Super::RestoreSaveState(Bytes);
	for (AActor* Linked : LinkedActors)
	{
		if (Linked && Linked->Implements<UToggleTarget>())
		{
			IToggleTarget::Execute_SetToggleState(Linked, bIsActive);
		}
	}
}

