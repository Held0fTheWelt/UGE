# GameFeature_InventorySystem

Inventory system. Manages item instances on pawns with replication, supports fragment-based extensions (equippable, stats, icons), and provides a world pickup component that can be picked up via the interaction system.

---

## Classes

### Item Core
| Class | Purpose |
|-------|---------|
| `UInventoryItemDefinition` | **Abstract** `UObject`, implements `IInventoryItemDefinitionInterface` — defines an item type via a `Fragments` array; base for all item definitions |
| `UInventoryItemFragment` | Abstract base for fragments — specialize per item type in Blueprint or C++ |
| `UInventoryItemInstance` | `UObject`, implements `IInventoryItemInstanceInterface` — runtime instance of an item; holds a replicated `StatTagStack` map |
| `UInventoryFunctionLibrary` | `UBlueprintFunctionLibrary` — `FindFragmentByClass()`, fragment data access |

### Components
| Class | Purpose |
|-------|---------|
| `UInventoryManagerComponent` | `UPawnComponent`, implements `IInventoryManagerInterface` — manages `TArray<UInventoryItemInstance>` with `FFastArraySerializer` replication; `AddItemDefinitionTyped()`, `RemoveItemInstance()` |
| `UWorldPickupComponent` | Extends `UCombinedBundleActorComponent`, implements `IInteractableTarget` and `IPickupable` — exposes an interaction slot; on pickup calls `UInventoryManagerComponent::AddItemDefinitionTyped()` on the instigator pawn |

### Subsystem
| Class | Purpose |
|-------|---------|
| `UInventorySubsystem` | `UWorldSubsystem` — tag-based inventory lookup by actor |

### Fragments
| Class | Purpose |
|-------|---------|
| `UInventoryFragment_EquippableItem` | SlotTag + `UEquipmentDefinition` class — links the item to the equipment system |
| `UInventoryFragment_SetStats` | Implements `IInventoryItemStatsInterface` — `InitialItemStats` map (tag → int32) |
| `UInventoryFragment_PickupIcon` | Pickup texture for world pickup display |
| `UInventoryFragment_QuickBarIcon` | QuickBar icon texture |

### Interfaces
| Interface | Purpose |
|-----------|---------|
| `IPickupable` | `OnPickedUp(AActor* Instigator)` — implemented by `UWorldPickupComponent` |

---

## Placement

| Where | What |
|-------|------|
| **Pawn** | `UInventoryManagerComponent` |
| **World actor** | `UWorldPickupComponent` (bundle component) |
| **DataAsset** | `UInventoryItemDefinition` subclasses with fragment configuration |

---

## Messages (GameplayMessageSubsystem)

| Tag | Direction | Struct |
|-----|-----------|--------|
| `InventoryMessageStackChanged` (dynamic) | Outgoing | `FInventoryChangeMessage` |

---

## Dependencies

| Module | Usage |
|--------|-------|
| `Core_Interfaces` | Inventory interfaces |
| `Core_Interact` | `IInteractableTarget`, `FInteractionOption` |
| `Elemental_Interfaces` | Pawn interfaces |
| `Elemental_Player` | PlayerState access |
| `Elemental_Structure` | Base structs |
| `GameplayAbilities` | Stat tag stacks |
| `GameplayMessageRuntime` | `FInventoryChangeMessage` broadcasting |
| `NetCore` | Replication helpers |
