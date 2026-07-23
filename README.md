# Mini-Interaction-Inventory-System

A reusable, data-driven interaction, inventory, and persistence framework for Unreal Engine 5.6, built as a plugin.

---

## Features

- **Generic interaction** – Any `Pawn` or `Character` can interact with actors through a single `IInteractable` interface. The same interaction flow works for pickups, containers, switches, doors, NPCs, and any custom interactable actor.

- **Decoupled inventory** – Inventory functionality is provided by a standalone `UInventoryComponent` and communicates through delegates rather than being tied to a specific character implementation.

- **Data-driven items** – Static item data lives in `UItemDefinition` data assets, while runtime inventory state is represented by lightweight `FInventoryEntry` structures.

- **Pluggable item behavior** – Item usage is implemented with inline-editable `UItemUseEffect` strategy objects. Different behaviors can be assigned directly from the item asset without creating Blueprint subclasses for every item.

- **Async-ready assets** – Item icons and pickup meshes use soft object references and are streamed only when required.

- **Generic save/load system** – Any actor derived from `ASaveableActorBase` automatically supports persistence. Only properties marked with `UPROPERTY(SaveGame)` are serialized, eliminating per-class save code.

- **Automation tested** – Inventory stacking, removal, and capacity behavior are covered by Unreal's Automation Framework and can be executed headlessly.

---

## Persistence

Persistence is completely independent of the interaction and inventory systems.

Actors deriving from `ASaveableActorBase` automatically capture and restore all properties marked with `UPROPERTY(SaveGame)` using Unreal's reflection system. No custom serialization code is required for each actor type.

---

## Getting Started

1. Copy the plugin into your project's `Plugins/` directory.
2. Enable the plugin from the Unreal Plugin Browser (or add it to your `.uproject`).
3. In **Project Settings → Asset Manager**, register a Primary Asset Type:
   - **Type:** `Item`
   - **Base Class:** `ItemDefinition`
   - **Directory:** e.g. `/Game/Items`
4. Create one or more `ItemDefinition` assets.
5. Add `UInventoryComponent` and `UInteractionComponent` to your player.
6. Bind your interaction input to `UInteractionComponent::TryInteract()`.
7. Bind `OnFocusChanged` to your UI to display interaction prompts.
8. Place actors such as:
   - `AItemPickupActor`
   - `AInteractiveContainerActor`
   - `AInteractiveSwitchActor`
   - `ADoorActor`

---

## Debugging

Use the following console command:

```text
Inventory.Dump
```

Prints the current player's inventory contents to the Output Log.

---

## Testing

Automation tests are included and can be executed through Unreal's Automation Framework.

Test group:

```text
Game.Inventory.*
```

---

## Design Notes

### Data vs Runtime State

- `UItemDefinition` stores static configuration.
- `FInventoryEntry` stores runtime inventory state.
- `ASaveableActorBase` stores runtime world state.

Configuration should remain in data assets, while only mutable runtime state should be serialized.

### Asset Loading

Icons and pickup meshes remain as soft references until explicitly requested. Assets are loaded asynchronously when needed instead of eagerly during startup.

---

## Known Limitations

- Only actors derived from `ASaveableActorBase` are automatically saved.
- Only properties marked with `UPROPERTY(SaveGame)` are serialized.
- Runtime-spawned actors are not recreated automatically during loading.
- Destroyed actors require a unique, persistent `SaveId` to remain removed across sessions.
- Multiplayer replication and multiplayer save/load are not currently supported.