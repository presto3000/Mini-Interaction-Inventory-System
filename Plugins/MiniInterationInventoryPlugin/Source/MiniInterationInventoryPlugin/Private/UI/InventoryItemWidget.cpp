// presto3000 Chris Pawlowski All Rights reserved.


#include "UI/InventoryItemWidget.h"

#include "UI/InventoryWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/AssetManager.h"
#include "Inventory/InventoryComponent.h"
#include "Items/ItemDefinition.h"

void UInventoryItemWidget::Setup(FPrimaryAssetId InID, int32 InQuantity, UInventoryWidget* Parent)
{
	ItemID = InID;
	Quantity = InQuantity;
	InventoryParent = Parent;

	UAssetManager& AssetManager = UAssetManager::Get();
	const FSoftObjectPath DefinitionPath = AssetManager.GetPrimaryAssetPath(ItemID);

	if (UItemDefinition* Definition = Cast<UItemDefinition>(DefinitionPath.TryLoad()))
	{
		if (ItemText)
		{
			ItemText->SetText(
				FText::Format(
					NSLOCTEXT("Inventory", "InventoryEntryFmt", "{0} x{1}"),
					Definition->DisplayName,
					FText::AsNumber(Quantity)
				)
			);
		}
	}
	InventoryComponent = Parent->Inventory;
}
void UInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UInventoryItemWidget::Clicked);
	}
	
	RefreshVisuals();
}

void UInventoryItemWidget::RefreshVisuals()
{
	if (UItemDefinition* Definition = UInventoryComponent::ResolveItemDefinition(ItemID))
	{
		if (ItemText)
		{
			ItemText->SetText(
				FText::Format(
					NSLOCTEXT("Inventory", "InventoryEntry", "{0} x{1}"),
					Definition->DisplayName,
					FText::AsNumber(Quantity)
				)
			);
		}
	}
	LoadIcon();
}

void UInventoryItemWidget::LoadIcon()
{
	if(!InventoryComponent)
	{
		return;
	}
	InventoryComponent->RequestLoadIcon(ItemID,[this](UTexture2D* Texture)
		{
			if (ItemIcon && Texture)
			{
				ItemIcon->SetBrushFromTexture(Texture);
			}
		}
	);
}

void UInventoryItemWidget::SetSelected(bool bInSelected)
{
	bSelected = bInSelected;
	UpdateSelectionVisual();
}

void UInventoryItemWidget::UpdateSelectionVisual()
{
	BP_OnSelectionChanged(bSelected);
}

void UInventoryItemWidget::Clicked()
{
	if(InventoryParent)
	{
		InventoryParent->SelectItem(ItemID);
	}
}
