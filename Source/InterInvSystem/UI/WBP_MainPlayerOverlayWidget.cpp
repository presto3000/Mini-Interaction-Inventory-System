// presto3000 Chris Pawlowski All Rights reserved.


#include "WBP_MainPlayerOverlayWidget.h"

#include "InterInvSystemCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Persistence/InteractionPersistenceLibrary.h"

void UWBP_MainPlayerOverlayWidget::OnSaveClicked()
{

	if (AInterInvSystemCharacter* Player = Cast<AInterInvSystemCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		UInteractionPersistenceLibrary::SaveWorldAndInventory(this,Player->InventoryComponent,TEXT("SaveSlot"));
	}
}

void UWBP_MainPlayerOverlayWidget::OnLoadClicked()
{
	if (AInterInvSystemCharacter* Player = Cast<AInterInvSystemCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		UInteractionPersistenceLibrary::LoadWorldAndInventory(this,Player->InventoryComponent,TEXT("SaveSlot"));
	}
}
