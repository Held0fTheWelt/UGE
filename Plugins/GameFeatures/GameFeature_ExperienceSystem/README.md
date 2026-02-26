# GameFeature_ExperienceSystem

Experience loading system. Loads and activates a `UExperienceDefinition` at runtime: registers GameFeature plugins, activates ability sets, and configures the game session. Provides a `UAsyncAction` Blueprint API to wait for load completion.

---

## Classes

### Components
| Class | Purpose |
|-------|---------|
| `UExperienceManagerComponent` | `UGameStateComponent`, implements `ILoadingProcessInterface` and `IExperienceManagerInterface` — orchestrates the experience loading process (`EExperienceLoadState` enum); activates/deactivates GameFeature plugins and ability sets; broadcasts `OnExperienceLoaded` delegates |

### Async Actions
| Class | Purpose |
|-------|---------|
| `UAsyncAction_ExperienceReady` | `UBlueprintAsyncActionBase` — BP node `WaitForExperienceReady`; fires the `OnReady` pin once the experience is fully loaded (also fires retroactively if already loaded) |

---

## Placement

| Where | What |
|-------|------|
| **GameState** | `UExperienceManagerComponent` |
| **DataAsset** | `UExperienceDefinition` (configured via GameMode or Level Blueprint) |

---

## Messages (GameplayMessageSubsystem)

None — the `OnExperienceLoaded` delegate on `UExperienceManagerComponent` is bound directly.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `GameFeatures` | `UGameFeaturesSubsystem` for activating/deactivating GF plugins |
| `GameplayTags` | Tag-based experience identification |
| `CommonLoadingScreen` | `ILoadingProcessInterface` — holds the loading screen until the experience is ready |
| `Elemental_Interfaces` | `IExperienceManagerInterface` |
| `CommonUser` | User initialization before experience start |
| `Core_Data` | `UExperienceDefinition` access |
