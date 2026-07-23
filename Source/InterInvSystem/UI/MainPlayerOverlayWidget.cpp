// presto3000 Chris Pawlowski All Rights reserved.


#include "MainPlayerOverlayWidget.h"

#include "InterInvSystemCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Persistence/InteractionPersistenceLibrary.h"

void UMainPlayerOverlayWidget::OnSaveClicked()
{

	if (AInterInvSystemCharacter* Player = Cast<AInterInvSystemCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		UInteractionPersistenceLibrary::SaveWorldAndInventory(this,Player->InventoryComponent,TEXT("SaveSlot"));
	}
}

void UMainPlayerOverlayWidget::OnLoadClicked()
{
	if (AInterInvSystemCharacter* Player = Cast<AInterInvSystemCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		UInteractionPersistenceLibrary::LoadWorldAndInventory(this,Player->InventoryComponent,TEXT("SaveSlot"));
	}
}
