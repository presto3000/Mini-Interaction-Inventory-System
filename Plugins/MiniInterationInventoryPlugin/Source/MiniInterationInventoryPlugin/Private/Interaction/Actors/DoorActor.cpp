// presto3000 Chris Pawlowski All Rights reserved.


#include "Interaction/Actors/DoorActor.h"


ADoorActor::ADoorActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ADoorActor::SetToggleState_Implementation(bool bNewState)
{
	bIsOpen = bNewState;
}

void ADoorActor::RestoreSaveState(const TArray<uint8>& Bytes)
{
	Super::RestoreSaveState(Bytes);

}

