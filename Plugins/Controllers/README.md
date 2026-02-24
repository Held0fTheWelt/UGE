# Controllers Plugin

Provides the player controller hierarchy for the UGE project. The plugin contains two classes: an abstract async-loading base (`AInputLoadingPlayerController`) and a full multi-entity implementation (`AUnifiedPlayerController`).

---

## Architecture

```
AModularPlayerController (Lyra/Engine)
  └── AInputLoadingPlayerController   ← async input asset loading
        └── AUnifiedPlayerController  ← full controller with interface dispatch
```

**Design principles:**
- Input assets are loaded asynchronously at startup, independent of possession
- Entity-type routing: each pawn declares what it is (`EPawnControlType`), the controller selects the correct input config
- All control calls go through registered component interfaces — no concrete casts
- Components self-register by calling `IRegisterComponentInterface` on the controller (e.g. on overlap/enter events)

---

## `AInputLoadingPlayerController`

Abstract base responsible for discovering and loading all `UInputConfigDataAssetSet` assets before any pawn is possessed.

### Loading Pipeline

```
BeginPlay
  └── LoadPrimaryAssets()
        └── UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(ADT_INPUT)
              └── OnPrimaryAssetLoaded(UInputConfigDataAssetSet*)
                    └── InputConfigDataMap[EInputSettingType] = asset
                    └── LoadMappingsAndActions()
                          └── UAssetManagedLoaderHelper::LoadInputAssetsAsync()
                                └── OnInputAssetsLoaded(actions, contexts)
                                      └── SetupEnhancedInputAction(action)  [per action]
                                      └── LoadCounter++
```

`AllInputAssetsLoaded()` returns `true` when `LoadCounter >= AssetsToLoadCount`.

### Key Members

| Member | Type | Description |
|--------|------|-------------|
| `CurrentControlType` | `EInputSettingType` | Active control profile |
| `InputConfigDataMap` | `TMap<EInputSettingType, UInputConfigDataAssetSet*>` | Loaded configs by type |
| `LoadCounter` | `int32` | Incremented per completed secondary load |
| `AssetsToLoadCount` | `int32` | Total secondary assets expected |

---

## `AUnifiedPlayerController`

Full controller implementation. Implements:
- `IRegisterComponentInterface` — component self-registration
- `IPlayerControllerInterface` — controller-level queries
- `ICameraAssistInterface` — hides pawn when camera penetrates
- `ITeamAgentInterface` — team ID delegation to player state

### Possession Flow

```
OnPossess(Pawn)
  ├── UnbindMappingContext()          ← clear timers, RegisteredComponentsByClass, InputSubsystem->ClearAllMappings()
  └── [poll until AllInputAssetsLoaded()]
        └── InitializeOnPosses()
              ├── IPawnControlInterface::GetPawnControlType()
              ├── UpdateSettingToLoad()   ← EPawnControlType → EInputSettingType
              └── [poll until InputConfigDataMap has the type]
                    ├── AddMappingContext to EnhancedInput subsystem
                    └── IPossessedByControllerInterface::Execute_PossessedByController()

OnUnPossess()
  ├── IPawnExtensionComponentInterface::UninitializeAbilitySystem()
  └── IScenePersistentPawnInterface::ShouldDespawnOnUnpossess() → destroy if true
```

### Entity Type Mapping

| `EPawnControlType` | `EInputSettingType` |
|--------------------|---------------------|
| PCT_HUMANOID | IST_HUMANOID |
| PCT_VEHICLE | IST_VEHICLE |
| PCT_HELICOPTER | IST_HELICOPTER |
| PCT_AIRCRAFT | IST_AIRCRAFT |
| PCT_TURRET | IST_TURRET |

---

## Component Registration System

Components (e.g. vehicle entry components, interaction zones) register themselves with the controller via `IRegisterComponentInterface`:

```cpp
// From any component, when the player enters or mounts:
IRegisterComponentInterface::Execute_RegisterComponentWithInterface(Controller, this);

// On exit or dismount:
IRegisterComponentInterface::Execute_UnregisterComponentWithInterface(Controller, this);
```

Internally, `NotifyComponentRegisterChanged` does:
1. Check which interfaces the component implements (against `ComponentInterfaceClasses`)
2. For each matching interface → add/remove from `RegisteredComponentsByClass`
3. On registration → call `AddInputMapping(EInputSettingType, EInputConfiguration)` to activate the relevant IMC

`GetInputSettingForClass` maps the interface class to the input config type:

| Interface Class | `EInputSettingType` |
|-----------------|---------------------|
| `UHumanoidControlInterface` | IST_HUMANOID |
| `UVehicleControlInterface` | IST_VEHICLE |
| `UHelicopterControlInterface` | IST_HELICOPTER |
| `UEngineControlInterface` | IST_CONTROL_ENGINE |
| `UEngineInputControlInterface` | IST_CONTROLS_ENGINE |
| `UCameraControlInterface` | IST_CONTROL_CAMERA |
| `UTurretControlInterface` | IST_TURRET |
| `UInteractControlInterface` | IST_INTERACT |

When a new component replaces an existing registration for the same interface, the old mapping contexts are removed and new ones are added automatically.

---

## Input Action Binding

`SetupEnhancedInputAction` binds each `UEnhancedInputAction` based on its `EInputActionType`:

| `EInputActionType` | Controller Method(s) | Trigger |
|--------------------|---------------------|---------|
| IAT_LOOK | `Look` | Triggered |
| IAT_TOGGLECAMERA | `ToggleCamera` | Started |
| IAT_ZOOMCAMERA | `ZoomCamera` | Triggered |
| IAT_INTERACT | `Interact` / `StopInteracting` | Started / Completed |
| IAT_MOVE | `Move` | Triggered |
| IAT_JUMP | `Jump` / `StopJumping` | Started / Completed |
| IAT_RUN | `Run` / `StopRunning` | Started / Completed |
| IAT_THROTTLE | `Throttle` | Triggered + Completed |
| IAT_STEER | `Steering` / `SteeringCompleted` | Triggered / Completed |
| IAT_BRAKE | `Brake` / `StartBrake` / `StopBrake` | Triggered / Started / Completed |
| IAT_HANDBRAKE | `StartHandbrake` / `StopHandbrake` | Started / Completed |
| IAT_RESET | `ResetVehicle` | Triggered |
| IAT_ROTATE | `Look` | Triggered |
| IAT_CYCLIC | `Cyclic` / `CyclicCompleted` | Started / Completed |
| IAT_CYCLIC_KEY_W/S/A/D | `CyclicKeyW/S/A/D Start/Stop` | Started / Completed |
| IAT_COLLECTIVE | `Collective` | Triggered + Completed |
| IAT_COLLECTIVE_KEY | `CollectiveKeyStart` / `CollectiveKeyStop` | Started / Completed |
| IAT_PEDALS | `Pedals` | Triggered + Completed |
| IAT_PEDALS_KEY_LEFT/RIGHT | `PedalsKeyLeft/RightStart/Stop` | Started / Completed |
| IAT_ENGINE_SWITCH | `StartEngineAction` / `StopEngineAction` | Started / Completed |
| IAT_ENGINE_2ND_SWITCH | `StartSecondEngineAction` / `StopSecondEngineAction` | Started / Completed |
| IAT_SAS_TOGGLE | `ToggleSASAction` | Started |

All handlers follow the same pattern:
```cpp
UActorComponent* Component = RegisteredComponentsByClass.FindRef(UXxxInterface::StaticClass());
if (!Component) { /* log VeryVerbose, return */ }
IXxxInterface::Execute_Method(Component, Value);
```

---

## Interact Logic

The `Interact()` handler uses a dual-path approach to distinguish world interaction from vehicle exit:

```
Interact()
  ├── No registered InteractControlInterface component?
  │     └── Pawn implements IPawnControlInterface?
  │           └── Execute_InteractOnPawn(Pawn, Controller)   ← exit vehicle
  │
  └── Component registered → CanInteractWithComponent(Component)?
        ├── Component owner == controlled pawn → true (exit-vehicle via Interact path)
        ├── Query IPhysicalSenseInterface::Execute_CanTouchActor on pawn sensing components
        │     ├── true → Execute_Interact(Component, Controller)    ← world interact
        │     └── false → Execute_InteractOnPawn(Pawn, Controller)  ← exit vehicle
        └── No sensing component → false
```

---

## Additional Controller Features

- **Auto-run**: `SetIsAutoRunning(bool)` sets/clears GAS tag `"Status.AutoRunning"`; `PlayerTick` injects movement when active
- **View rotation replication**: tick replicates via `APlayerState_Elemental::SetReplicatedViewRotation`
- **Force feedback**: connected through `ISharedSettingsForceFeedbackInterface` in `SetPlayer`
- **Camera penetration**: `ICameraAssistInterface::OnCameraPenetratingTarget()` hides the pawn for one frame
- **Cheat commands**: `ServerCheat` / `ServerCheatAll` RPCs (server-only)

---

## Log Categories

| Category | Usage |
|----------|-------|
| `Log_AssetManagedController` | Base asset loader |
| `Log_InputLoadingController` | Primary/secondary asset loading |
| `Log_UnifiedController` | General possession / init |
| `Log_UnifiedController_Setup` | Action binding during setup |
| `Log_UnifiedController_Register` | Component registration / mapping context changes |
| `Log_UnifiedController_Controls` | Per-frame input dispatch |

---

## Dependencies

| Plugin / Module | Purpose |
|-----------------|---------|
| AssetLoader | Async discovery and loading of `UInputConfigDataAssetSet` |
| InputSystem | `UEnhancedInputAction`, `UEnhancedInputMappingContext`, `EInputActionType`, `EInputSettingType` |
| Elemental_Classes | `AModularPlayerController` base |
| Elemental_Interfaces | Control interfaces (Camera, Humanoid, Vehicle, Helicopter, etc.) |
| Elemental_Player | Player state / team integration |
| EnhancedInput | `UEnhancedInputComponent`, `UEnhancedInputLocalPlayerSubsystem` |
| GameplayAbilities | GAS / `IAbilitySystemInputInterface` for `PostProcessInput` |
| CommonInput | Shared input settings integration |
