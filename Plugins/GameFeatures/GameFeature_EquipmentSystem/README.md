# GameFeature_EquipmentSystem

Equipment system. Manages equipped items on pawns (weapons, gadgets, etc.): spawns actors, grants ability sets, and replicates state. `UQuickBarComponent` provides the controller with a slot-based selection interface.

---

## Classes

### Equipment Core
| Class | Purpose |
|-------|---------|
| `UEquipmentDefinition` | **Abstract** `UObject` — defines a piece of equipment: `AbilitySetsToGrant`, `ActorsToSpawn` (list of `FEquipmentActorToSpawn`) |
| `UEquipmentInstance` | `UObject` — runtime instance of an equipped item; replicated; BP events `OnEquipped` / `OnUnequipped` |
| `UEquipmentManagerComponent` | `UPawnComponent`, implements `IEquipmentManagerInterface` — manages the equipment list with replication (`FFastArraySerializer`); equip/unequip logic, ability grant/revoke |

### Quick Bar
| Class | Purpose |
|-------|---------|
| `UQuickBarComponent` | `UControllerComponent` — slot-based quick bar with configurable slot count; replicates active index; broadcasts `QuickBar.Message.SlotsChanged` and `QuickBar.Message.ActiveIndexChanged` |

### Abilities
| Class | Purpose |
|-------|---------|
| `UGameplayAbility_FromEquipment` | Extends `UGameplayAbility` — base for abilities granted by equipment; provides access to `UEquipmentInstance` and `UInventoryItemInstance` |

---

## Placement

| Where | What |
|-------|------|
| **Pawn** | `UEquipmentManagerComponent` |
| **PlayerController** | `UQuickBarComponent` |
| **DataAsset** | `UEquipmentDefinition` subclass per equipment type |

---

## Messages (GameplayMessageSubsystem)

| Tag | Direction | Struct |
|-----|-----------|--------|
| `QuickBar.Message.SlotsChanged` | Outgoing | `FQuickBarSlotsChangedMessage` |
| `QuickBar.Message.ActiveIndexChanged` | Outgoing | `FQuickBarActiveIndexChangedMessage` |

---

## Dependencies

| Module | Usage |
|--------|-------|
| `Niagara` | VFX during equip (ActorsToSpawn may include Niagara systems) |
| `GameplayAbilities` | Ability grant/revoke, `UGameplayAbility` |
| `Elemental_Interfaces` | Pawn and equipment interfaces |
| `Elemental_MessageSystem` | `GameplayMessageSubsystem` for QuickBar messages |
| `GameplayMessageRuntime` | `UGameplayMessageSubsystem` |
| `IrisCore` | Iris replication |
