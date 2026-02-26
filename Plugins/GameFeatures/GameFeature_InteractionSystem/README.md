# GameFeature_InteractionSystem

GAS-based interaction system. Detects `IInteractableTarget` objects near the player via ability tasks (line trace or overlap), dynamically grants interaction abilities, and communicates progress via the `GameplayMessageSubsystem`.

---

## Classes

### Interfaces
| Interface | Purpose |
|-----------|---------|
| `IInteractionInstigator` | Implemented by interacting actors — provides `GetInteractionInstigatorActor()` |

### Abilities
| Class | Purpose |
|-------|---------|
| `UGameplayAbility_Interact` | **Abstract** `UGameplayAbility` — base class for interaction abilities; holds `CurrentOptions` (`TArray<FInteractionOption>`); calls `TriggerInteraction()`; subclass in Blueprint |
| `AGameplayAbilityTargetActor_Interact` | Extends `AGameplayAbilityTargetActor_Trace` — intermediate base for interaction targeting |

### Ability Tasks
| Class | Purpose |
|-------|---------|
| `UAbilityTask_GrantNearbyInteraction` | Grants interaction abilities to nearby `IInteractableTarget` objects (overlap-based); cleaned up when task ends |
| `UAbilityTask_WaitForInteractableTargets` | **Abstract** — monitors changes to interactable targets; fires `FInteractableObjectsChangedEvent` |
| `UAbilityTask_WaitForInteractableTargets_SingleLineTrace` | Concrete implementation via single line trace; configurable (channel, length, interval) |

### Utilities
| Class | Purpose |
|-------|---------|
| `UInteractionStatics` | `UBlueprintFunctionLibrary` — `GetInteractableTargetsFromActor()`, `AppendInteractableTargetsFromHitResult()` |

### Structs / Messages
| Type | Purpose |
|------|---------|
| `FInteractionDurationMessage` | Message with progress (0..1) and `InteractableTarget` — broadcast during an active interaction |

---

## Placement

| Where | What |
|-------|------|
| **Pawn (via ASC)** | Blueprint subclass of `UGameplayAbility_Interact` as a startup ability |
| **Interactable actor** | Implements `IInteractableTarget` (e.g. via `UWorldPickupComponent`) |

---

## Messages (GameplayMessageSubsystem)

| Tag | Direction | Struct |
|-----|-----------|--------|
| `Interaction.Duration.Message` | Outgoing | `FInteractionDurationMessage` |

---

## Dependencies

| Module | Usage |
|--------|-------|
| `GameplayAbilities` | `UGameplayAbility`, `UAbilityTask`, `AGameplayAbilityTargetActor` |
| `GameplayTasks` | Ability task base classes |
| `Core_Interfaces` | Project-specific interfaces |
| `Core_Interact` | `IInteractableTarget`, `FInteractionOption` |
| `Elemental_Interfaces` | Pawn interfaces |
| `UMG` | HUD progress display |
