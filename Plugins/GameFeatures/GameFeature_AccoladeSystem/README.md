# GameFeature_AccoladeSystem

Kill/streak/assist feedback system. Processes elimination and damage messages and displays data-driven accolade toasts in the HUD.

---

## Classes

### Message Processors
Placed as components on the **GameState** (or via GameFeature `AddComponents`).

| Class | Purpose |
|-------|---------|
| `UAssistProcessor` | Tracks damage per attacker; broadcasts `Assist.Message` to all non-killers when a target dies |
| `UElimChainProcessor` | **Abstract** — tracks rapid kill chains (time window: `ChainTimeLimit`, default 4.5 s). Subclass in Blueprint and populate `ElimChainTags` (`int32 → FGameplayTag`) |
| `UElimStreakProcessor` | **Abstract** — tracks kill streaks without own death. Subclass in Blueprint and populate `ElimStreakTags` |

### Widget
| Class | Purpose |
|-------|---------|
| `UAccoladeHostWidget` | `UCommonUserWidget`. Listens for `Accolade.Message`, async-loads icon/sound from DataRegistry, calls BP events `CreateAccoladeWidget` / `DestroyAccoladeWidget`. `LocationName` tag controls which accolades are displayed |

### Data
| Class | Purpose |
|-------|---------|
| `FAccoladeDefinitionRow` | DataTable row: display name, sound, icon, duration, `LocationTag`, suppression rules |
| `UAccoladeDefinition` | Standalone DataAsset variant (no DataTable required) |

---

## Placement

| Where | What |
|-------|------|
| **GameState** | `UAssistProcessor`, Blueprint subclass of `UElimChainProcessor`, Blueprint subclass of `UElimStreakProcessor` |
| **HUD widget** | `UAccoladeHostWidget` (one instance per display location, different `LocationName` per instance) |
| **DataRegistry / DataTable** | Catalog of `FAccoladeDefinitionRow` entries |

---

## Messages (GameplayMessageSubsystem)

| Tag | Direction | Struct |
|-----|-----------|--------|
| `WarCollection.Elimination.Message` | Incoming | `FVerbMessage` |
| `Core.Damage.Message` | Incoming | `FVerbMessage` |
| `Assist.Message` | Outgoing | `FVerbMessage` |
| `Accolade.Message` | Outgoing | `FNotificationMessage` (consumed by `UAccoladeHostWidget`) |

---

## Dependencies

| Module | Usage |
|--------|-------|
| `Elemental_MessageSystem` | `UGameplayMessageProcessor`, `FNotificationMessage`, `UVerbMessageHelpers` |
| `Elemental_Structure` | `FVerbMessage` |
| `CommonUI` | `UCommonUserWidget` |
| `DataRegistry` | Async loading of accolade assets |
| `AsyncMixin` | Loading helper for `UAccoladeHostWidget` |
