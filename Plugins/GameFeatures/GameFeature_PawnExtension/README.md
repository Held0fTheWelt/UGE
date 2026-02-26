# GameFeature_PawnExtension

Pawn initialization coordinator. Ensures all pawn subsystems (AbilitySystemComponent, input, PawnData) are initialized in the correct order — including async loading and late-joining scenarios. Implements the `IGameFrameworkInitStateInterface` protocol.

---

## Classes

### Components
| Class | Purpose |
|-------|---------|
| `UPawnExtensionComponent` | `UPawnComponent`, implements `IGameFrameworkInitStateInterface` and `IPawnExtensionComponentInterface` — central coordinator for pawn initialization; manages `PawnData`, binds the ASC, processes input configuration; exposes `OnAbilitySystemInitialized` / `OnAbilitySystemUninitialized` delegates |

---

## InitState Protocol

The component progresses through four states in order (all from `GameplayTags_Core`):

| Tag | Meaning |
|-----|---------|
| `InitState.Spawned` | Actor is spawned, basic setup complete |
| `InitState.DataAvailable` | PawnData and dependencies are present |
| `InitState.DataInitialized` | ASC and all systems initialized |
| `InitState.GameplayReady` | Pawn is ready for gameplay |

Other components (input, ability grants, cosmetics) listen for these states and initialize only once their prerequisites are met.

---

## Placement

| Where | What |
|-------|------|
| **Pawn** | `UPawnExtensionComponent` (must be early in the component stack) |

---

## Messages (GameplayMessageSubsystem)

None — communication via `IGameFrameworkInitStateInterface` delegates and direct component references.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `ModularGameplay` | `IGameFrameworkInitStateInterface`, `UGameFrameworkComponentManager` |
| `GameplayAbilities` | ASC binding |
| `GameplayTags` | InitState tags |
| `Core_Structure` | `GameplayTags_Core` (InitState.* tags) |
| `Elemental_Classes` | Elemental base classes |
| `Elemental_Interfaces` | `IPawnExtensionComponentInterface` |
| `Elemental_Structure` | Base structs |
