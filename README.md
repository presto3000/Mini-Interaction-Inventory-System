# Mini-Interaction-Inventory-System

Mini Interaction Inventory System U.E 5.6

A reusable, data-driven interaction and inventory framework for Unreal Engine 5, built as a plugin.

## Highlights

Generic interaction - any Pawn or Character can detect and interact with any actor in front of it through a single `IInteractable` interface. Works identically for pickups, containers, switches, doors, or NPCs.
Decoupled inventory - a standalone `UInventoryComponent`, not baked into any character class, communicating entirely through delegates.
Data/runtime separation - item definitions (`UItemDefinition`) are plain data assets; runtime state (`FInventoryEntry`) is just an ID and a quantity.
Instanced strategy objects for item behavior - "what happens when I use this potion" is a small, inline-editable `UItemUseEffect` object picked from a dropdown directly on the item asset. No Blueprint subclassing required per item.
Async-friendly by design - icons and pickup meshes are soft references, streamed in on demand instead of loading eagerly.
Full save/load support - any interactable actor can opt into persistence with a couple of `UPROPERTY(SaveGame)` tags and a stable GUID; no per-class serialization code required.
Automation-tested - stacking, removal, and capacity-limit behavior are covered by engine automation tests, runnable headlessly.

Persistence sits orthogonally to all of this: anything deriving from `ASaveableActorBase` gets generic capture/restore for free, through reflection over its own `SaveGame`-tagged properties — the save system never hardcodes a specific actor class.

## Getting started

Drop this plugin's folder into your project's `Plugins/` directory and enable it from the Plugins browser (or add it to your `.uproject`'s plugin list directly).
Project Settings -> Game -> Asset Manager, add a Primary Asset Type:
Type: `Item`
Base Class: `ItemDefinition`
Directory: wherever you'll keep your item assets (e.g. `/Game/Items`)
Create a few `ItemDefinition` Data Assets. Set their stack rules, and - if you want the item to do something when used - pick a `UseEffect` from the inline dropdown (`Heal`, `Print Debug Message`, or your own subclass) and fill in its fields. No new class needed.
Add `UInventoryComponent` and `UInteractionComponent` to your player Pawn (Blueprint "Add Component" or C++ `CreateDefaultSubobject`).
Bind an input action to `UInteractionComponent::TryInteract`, and bind `OnFocusChanged` in your HUD to show interaction prompts.
Place `AItemPickupActor`, `AInteractiveContainerActor`, `AInteractiveSwitchActor`, or `ADoorActor` instances on your level.

## Debugging

`Inventory.Dump` - prints the local player's current inventory contents to the log.

## Testing
Automation tests live alongside the source and run through the standard Unreal automation framework (`Game.Inventory.*` test group) 
Design notes
A few deliberate choices worth calling out for anyone extending this:

Soft references stay soft until actually needed. Icons and pickup meshes only resolve when something explicitly asks for them (`RequestLoadIcon`, or on `BeginPlay` for a dropped pickup's mesh).
Save state and config data never mix. `FInventoryEntry` and `ASaveableActorBase`'s captured bytes describe what changed; `UItemDefinition` and level-placed actor defaults describe what's possible. If you find yourself serializing something that's actually just static config, it belongs on the data asset instead.

## Known Limitations

- Only actors derived from `ASaveableActorBase` are automatically saved.
- Only properties marked with `UPROPERTY(SaveGame)` are serialized.
- Dynamically spawned actors are not recreated after loading.
- Destroyed actors must have a persistent `SaveId` to be correctly identified across sessions.