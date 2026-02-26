# GameFeature_HeroSystem

Pawn input and camera coordinator for player-controlled characters. `UHeroComponent` hooks into the `IGameFrameworkInitStateInterface` protocol and handles two responsibilities: forwarding input bindings to the ASC and managing ability-driven camera mode overrides.

---

## Classes

### Components
| Class | Purpose |
|-------|---------|
| `UHeroComponent` | `UPawnComponent`, implements `IGameFrameworkInitStateInterface` — coordinates input binding and camera override for player-controlled pawns |

---

## InitState Protocol

`UHeroComponent` follows the standard 4-state protocol and depends on `UPawnExtensionComponent`:

| Transition | Action |
|------------|--------|
| `DataAvailable → DataInitialized` | Calls `IPawnExtensionComponentInterface::Execute_InitializeAbilitySystem()`; starts input binding via `NAME_BindInputsNow` extension event |
| `DataInitialized → GameplayReady` | Pawn is fully ready for gameplay |

**`NAME_BindInputsNow`** — extension event broadcast to both Pawn and PlayerController. Other components (e.g. input setup components) listen for this event and bind their actions when ready.

---

## Key Methods

| Method | Purpose |
|--------|---------|
| `SetAbilityCameraMode(CameraMode, SpecHandle)` | Overrides the active camera mode from within an ability |
| `ClearAbilityCameraMode(SpecHandle)` | Clears the camera override (only if `SpecHandle` matches) |
| `IsReadyToBindInputs()` | Returns whether input bindings have already been applied |
| `Input_AbilityInputTagPressed/Released(Tag)` | Forwards input events to `IAbilitySystemInputInterface` on the ASC owner |

---

## Placement

| Where | What |
|-------|------|
| **Pawn** | `UHeroComponent` |
| **Pawn** | `UPawnExtensionComponent` (prerequisite — must be present) |
| **PlayerState** | AbilitySystemComponent (prerequisite) |

---

## Messages (GameplayMessageSubsystem)

None — communication exclusively via `IGameFrameworkInitStateInterface` delegates and direct component references.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `ModularGameplay` | `IGameFrameworkInitStateInterface`, extension events |
| `GameplayAbilities` | ASC access, `FGameplayAbilitySpecHandle` |
| `GameplayTags` | Input tag routing |
| `Elemental_Interfaces` | `IPawnExtensionComponentInterface`, `IAbilitySystemInputInterface` |
| `Elemental_Common` | General utility types |
