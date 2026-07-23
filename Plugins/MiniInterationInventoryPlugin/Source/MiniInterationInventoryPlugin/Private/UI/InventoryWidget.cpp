// presto3000 Chris Pawlowski All Rights reserved.


#include "UI/InventoryWidget.h"

#include "UI/InventoryItemWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Inventory/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UseButton->SetVisibility(ESlateVisibility::Collapsed);
	RemoveButton->SetVisibility(ESlateVisibility::Collapsed);
	
	APawn* Pawn = GetOwningPlayerPawn();
	if (Pawn)
	{
		Inventory = Pawn->FindComponentByClass<UInventoryComponent>();
	}
	
	if (UseButton)
	{
		UseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnUseClicked);
	}

	if (RemoveButton)
	{
		RemoveButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnRemoveClicked);
	}
	
	if (Inventory)
	{
		Inventory->OnInventoryChanged.AddDynamic(this,	&UInventoryWidget::InventoryChanged);
		RefreshInventory();
	}
}

void UInventoryWidget::InventoryChanged()
{
	RefreshInventory();
	
	SelectedItem = FPrimaryAssetId();

	UseButton->SetVisibility(ESlateVisibility::Collapsed);
	RemoveButton->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryWidget::SelectItem(FPrimaryAssetId ItemId)
{
	SelectedItem = ItemId;

	if (UseButton)
	{
		UseButton->SetVisibility(ESlateVisibility::Visible);
	}

	if (RemoveButton)
	{
		RemoveButton->SetVisibility(ESlateVisibility::Visible);
	}

	for (UInventoryItemWidget* Widget : ItemWidgets)
	{
		if (!Widget)
		{
			continue;
		}

		Widget->SetSelected(Widget->GetItemId() == ItemId);
	}
}

void UInventoryWidget::OnUseClicked()
{
	if (!Inventory)
		return;
	
	if (!SelectedItem.IsValid())
		return;
	
	Inventory->UseItem(SelectedItem);
}



void UInventoryWidget::OnRemoveClicked()
{
	if(!Inventory)
		return;
	
	if(!SelectedItem.IsValid())
		return;
	
	Inventory->RemoveItem(SelectedItem,	1);
}


void UInventoryWidget::RefreshInventory()
{
	if (!InventoryList || !Inventory)
		return;
	
	InventoryList->ClearChildren();
	if (ItemWidgetClass)
	{
		for (const FInventoryEntry& Entry : Inventory->GetEntries())
		{
			UItemDefinition* Definition = UInventoryComponent::ResolveItemDefinition(Entry.ItemId);

			if (!Definition)
			{
				continue;
			}
			// Create item row widget
			if (UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this,ItemWidgetClass))
			{
				ItemWidget->Setup(Entry.ItemId,	Entry.Quantity,this);
				InventoryList->AddChild(ItemWidget);
				ItemWidgets.Add(ItemWidget);
			}
		}
	}
}