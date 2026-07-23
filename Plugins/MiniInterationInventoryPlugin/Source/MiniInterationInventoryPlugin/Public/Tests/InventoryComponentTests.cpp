#include "Inventory/InventoryComponent.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FInventoryOperationsTest,
	"Game.Inventory.AddRemoveStacking",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter
)

bool FInventoryOperationsTest::RunTest(const FString& Parameters)
{
	// Item registered in Asset Manager:
	// MaxStackSize = 5
	const FPrimaryAssetId TestItemId(TEXT("Item"), TEXT("DA_Potion_HP"));
	UInventoryComponent* Inventory = NewObject<UInventoryComponent>(GetTransientPackage());
	Inventory->MaxSlots = 5;
	int32 AmountAdded = 0;

	// ----------------------------
	// ADD TEST
	// ----------------------------

	EInventoryOpResult Result =	Inventory->AddItem(TestItemId, 3, AmountAdded);

	TestEqual(TEXT("Adding item succeeds"),Result,EInventoryOpResult::Success);
	TestEqual(TEXT("Correct amount added"),AmountAdded,3);
	TestEqual(TEXT("Inventory contains added items"),Inventory->GetQuantityOf(TestItemId),3);
	
	// ----------------------------
	// STACKING TEST
	// ----------------------------

	Result = Inventory->AddItem(TestItemId, 4, AmountAdded);

	TestTrue(TEXT("Adding over stack size succeeds or partially succeeds"),
		Result == EInventoryOpResult::Success ||
		Result == EInventoryOpResult::Partial
	);

	TestEqual(TEXT("Total quantity after stacking"),Inventory->GetQuantityOf(TestItemId),7);
	TestEqual(TEXT("Overflow creates second stack"),Inventory->GetEntries().Num(),2);
	
	// ----------------------------
	// REMOVE PART OF STACK TEST
	// ----------------------------

	Result = Inventory->RemoveItem(TestItemId, 2);
	
	TestEqual(TEXT("Removing part of stack succeeds"),Result,EInventoryOpResult::Success);
	TestEqual(TEXT("Quantity after partial removal"),Inventory->GetQuantityOf(TestItemId),5);
	
	// ----------------------------
	// REMOVE ALL TEST
	// ----------------------------

	Result = Inventory->RemoveItem(TestItemId, 999);

	TestEqual(TEXT("Removing more than owned reports failure"),Result,EInventoryOpResult::Failed_NotFound);
	TestEqual(TEXT("All items removed"),Inventory->GetQuantityOf(TestItemId),0);
	TestEqual(TEXT("Inventory entries empty"),Inventory->GetEntries().Num(),0);
	
	// ----------------------------
	// REMOVE MISSING ITEM TEST
	// ----------------------------
	
	Result = Inventory->RemoveItem(TestItemId, 1);
	TestEqual(TEXT("Removing missing item fails"),Result,EInventoryOpResult::Failed_NotFound);
	
	return true;
}

#endif
