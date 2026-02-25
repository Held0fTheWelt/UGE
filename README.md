# Unified Game Equipment (UGE)

Unreal Engine **5.6** — Modular, plugin-based game framework built on Lyra foundations.
Provides a reusable equipment system for humanoids, helicopters, vehicles, ships, and aircraft with a unified controller, physics-driven flight, GAS integration, and data-driven configuration.

# Architecture Overview

The project is structured in dependency layers. Lower layers have no knowledge of higher ones.

```
┌─────────────────────────────────────────────────────────┐
│  Game Features          GameFeature_HumanoidMovement    │
│                         GameFeature_PhysicalAwareness   │
├─────────────────────────────────────────────────────────┤
│  Equipment Plugins      Humanoids · Helicopters ·       │
│                         Vehicles · Ships · Aircrafts    │
├─────────────────────────────────────────────────────────┤
│  Controllers            AUnifiedPlayerController        │
├─────────────────────────────────────────────────────────┤
│  Core_Collection        GAS · Experience · Settings ·   │
│  (20 modules)           Performance · Replay · Editor   │
├─────────────────────────────────────────────────────────┤
│  Elemental_Collection   Base classes · Interfaces ·     │
│  (12 modules)           Networking · Player · Teams     │
├─────────────────────────────────────────────────────────┤
│  InputSystem            Enhanced Input · IMC · Enums    │
│  AssetLoader            Async soft-ref asset loading    │
└─────────────────────────────────────────────────────────┘
```

---

## Plugins

### Foundation Layer

#### AssetLoader
Async soft-reference asset loading. Extends `UAssetManager` with a startup job queue and thread-safe asset tracking via `UPreLoadingDataAsset`. All data assets that need to be pre-loaded at startup inherit from this base class.

#### InputSystem
Custom Enhanced Input layer on top of UE's `EnhancedInput` plugin.
- `EInputActionType` enum — typed input actions (Look, Move, Jump, Cyclic, Collective, Pedals, Shoot, …)
- `UEnhancedInputMappingContext` — wraps `UInputMappingContext` and tags contexts by `EInputConfiguration`
- `IInputControllerInterface` — query interface for the active input configuration
- `EInputSettingType` / `EInputConfiguration` — binds input context selection to pawn type and platform

---

### Core Layer

#### Elemental_Collection *(12 modules)*
Shared base classes and interfaces consumed by all higher layers.

| Module | Responsibility |
|--------|---------------|
| `Elemental_Classes` | Base actor/pawn/character classes (`ACombinedBundleActor/Pawn/Character`) |
| `Elemental_Interfaces` | Core interfaces (`IAssetTypeInterface`, `IShapeUpdatedInterface`, `IPawnControlInterface`, …) |
| `Elemental_Data` | Shared data assets (`UMeshSetupDataAsset`, `USkeletalMeshSetupDataAsset`, …) |
| `Elemental_Structure` | Shared structs and type definitions |
| `Elemental_Logging` | `GlobalLog`, `StructuredLog` (UE_LOGFMT macros) |
| `Elemental_Network` | Replication helpers and shared network structs |
| `Elemental_Common` | General-purpose utility types |
| `Elemental_Settings` | Settings base types |
| `Elemental_Player` | Player-specific base classes |
| `Elemental_Teams` | Team system |
| `Elemental_MessageSystem` | Message-passing utilities |
| `Elemental_Editor` | Editor-only support tools |

#### Core_Collection *(20 modules)*
Lyra-derived game framework. Provides the full lifecycle of an online multiplayer game.

| Group | Modules |
|-------|---------|
| **Foundation** | `Core_Structure`, `Core_Interfaces`, `Core_Logging` |
| **Data & Settings** | `Core_Data`, `Core_Setup`, `Core_Performance` |
| **Game Framework** | `Core_Classes` (`ACoreCharacter`, `ACoreGameState`, `UCoreWorldSettings`), `Core_System` (`UCoreGamePhaseSubsystem`), `Core_GameFeatures` (Experience system, modular gameplay actions) |
| **Gameplay** | `Core_AbilitySystem` (GAS — ability sets, attribute sets, cue managers, tag relationships), `Core_Interact` (interaction query/option system), `Core_Physics` (`UCorePhysicalMaterialWithTags`), `Core_Effects` |
| **Infrastructure** | `Core_HotfixSystem` (`UCoreHotfixManager`), `Core_Replay` (`UWarCollectionReplaySubsystem`), `Core_Tests` (Gauntlet HTTP endpoints), `Core_Development` |
| **Auxiliary** | `Core_Demo`, `Core_Extras` |
| **Editor** | `Core_Collection_Editor` (asset validators, property customization, `UCoreEditorEngine`, P4 commandlet) |

Key systems:
- **Experience system** — `UCoreWorldSettings.DefaultGameplayExperience` → loads ability sets, input contexts, widgets, and splitscreen config as modular Game Feature actions
- **GAS** — `ACoreCharacter` has a full Gameplay Ability System with attribute sets and a compressed fast-share replication path
- **`UCoreGamePhaseSubsystem`** — hierarchical tag-driven game phase management with sibling cancellation
- **`UGlobalAbilitySubsystem`** — applies abilities/effects world-wide to all registered ASCs

---

### Controller Layer

#### Controllers
Two-class player controller hierarchy.

```
APlayerController
  └─ UAssetManagedController    — soft-ref data asset loading
       └─ UInputLoadingController  — Enhanced Input setup
            └─ AUnifiedPlayerController  — component registration + control dispatch
```

- **Component registration system** — components register themselves by interface class; the controller automatically selects the correct `UInputMappingContext` and binding set
- **`GetPawnControlType()`** — the possessed pawn returns `EPawnControlType` (PCT_HUMANOID, PCT_HELICOPTER, PCT_VEHICLE, …); the controller dispatches the matching input actions
- **Control regions** — Camera, Humanoid (Move/Jump/Run), Vehicle (Steer/Brake/Handbrake/Reset), Helicopter (Cyclic/Collective/Pedals/SAS), Turret, Engine start/stop
- **Interact** — dual-path: exit-vehicle if pawn is a vehicle; otherwise `IPawnControlInterface::Execute_InteractOnPawn`

---

### Equipment Plugins

#### Humanoids
Concrete `AHumanoidCharacter` (`ACombinedBundleCharacter` subclass).
- Capsule: 55 cm radius × 96 cm half-height
- Returns `PCT_HUMANOID` → triggers humanoid input binding in the controller
- Enables overlap on `ECC_GameTraceChannel2` (QueryCollision) for the interact and sense system
- `bSetMeshToDefaultSkeletalMeshComponent = false` — mesh assignment left to Blueprint subclasses

#### Helicopters
Full helicopter framework with physics-driven flight and data-driven configuration.

**Class hierarchy (parallel Actor/Pawn × SM/SKM):**
```
ACombinedBundleActor/Pawn
  └─ AHelicopterActor/Pawn          — base (PCT_HELICOPTER)
       ├─ ASM_Helicopter*            — StaticMesh root
       └─ ASKM_Helicopter*           — SkeletalMesh root + async asset loading
```

**Key interfaces:**

| Interface | Purpose |
|-----------|---------|
| `IHelicopterControlsInterface` | 4 delegates: Collective, Cyclic (2D), Pedals, Throttle |
| `IHelicopterEngineInterface` | 9 delegates: rotor speeds, lift, thrust, drag, torque, lift vector |
| `IHelicopterPhysicsInterface` | Aileron angle broadcast |
| `IHelicopterSASInterface` | `ToggleSAS` / `IsSASEnabled` / `GetSASHoldMode` (GameFeature component) |
| `IHelicopterCollectiveOffsetProviderInterface` | Auto-hover PID collective offset (P6.2) |
| `IHelicopterTrimNotifyInterface` | Cyclic-released event for auto-trim capture (P6.3) |
| `IHelicopterAnimInstanceInterface` | Main/tail rotor speed delegates for AnimBP |

**Data assets:**
- `UHeliFlightPresetDataAsset` — per flight-mode (`Realistic` / `SAS_Assist` / `Arcade`) physics config: disc tilt, drag, damping, SAS PID gains, cyclic torque, reactive main-rotor torque, center-of-mass override
- `UPhysicsBasedHelicopterDefinitionDataAsset` — rig bone mapping (body, main/tail rotor, swashplate, aileron), torque multiplier, angular velocity cap, cyclic gain calibration, mass override

**`UHelicopterAnimInstance`** — dual-path: legacy delegate binding or direct-setter push from `SKM_PhysicsBasedHelicopterComponent`, computes 9 output values for AnimBP Modify-Bone nodes (rotor yaw/pitch, disc tilt, swashplate rotation, aileron angle).

#### Vehicles
Chaos-based wheeled vehicle support.

| Module | Type | Purpose |
|--------|------|---------|
| `Vehicles` | Runtime | Vehicle actor/pawn classes, movement component integration |
| `VehiclesEditor` | Editor | Vehicle editor tooling |

Depends on `ChaosVehiclesPlugin`, `AssetLoader`, `Elemental_Collection`, `Core_Collection`.

#### Ships
Ship actor/pawn classes.

| Module | Type |
|--------|------|
| `Ships` | Runtime |

Depends on `Core_Collection`.

#### Aircrafts
Fixed-wing aircraft actor/pawn classes.

| Module | Type |
|--------|------|
| `Aircrafts` | Runtime |

Depends on `AssetLoader`, `Elemental_Collection`, `Core_Collection`, `LoggingMacros`.

---

### Game Feature Plugins

Located in `Plugins/GameFeatures/`. Loaded explicitly after project startup (`ExplicitlyLoaded: true`).

#### GameFeature_HumanoidMovement
Adds humanoid movement mechanics (walking, running, jumping animations and input binding) to `AHumanoidCharacter` via the modular Game Feature action system.
Module: `GameFeature_HumanoidMovementRuntime` (Runtime, Default).
State: **Active by default** (`BuiltInInitialFeatureState: Active`).

#### GameFeature_PhysicalAwareness
Collision-driven touch-sense system. Lets any pawn detect, track, and query physically overlapping actors.
Module: `GameFeature_PhysicalAwarenessRuntime` (Runtime, Default).
State: **Active by default** (`BuiltInInitialFeatureState: Active`).

**Core component — `UPhysicalAwarenessSenseComponent`** (`UPawnDrivingActorComponent_Base` + `IPhysicalSenseInterface`):
- Retrieves the pawn's collision primitive via `ISceneComponentCatchInterface` ("Collision" slot)
- On overlap begin: if the overlapping component implements `IInteractControlInterface` and does **not** carry `INoTouchCollisionInterface`, the actor is added to `TouchableActors` via `RegisterObject()`
- On overlap end: actor removed via `ForgetObject()`, `CurrentTouchedActor` cleared if needed
- `IPhysicalSenseInterface` query surface: `CanTouchActor()`, `CanTouchCurrentActor()`, `IsTouchingCurrentActor()`, `SetTouchingCurrentActor()`, `GetCurrentTouchActor()`, `GetTouchableActors()`
- All tracking can be enabled/disabled at runtime via `SetCollisionComponentActive()`

**Data asset — `UPhysicalAwarenessSenseDataAsset`** (Primary asset type `"PhysicalAwareness"`, extends `UPreLoadingDataAsset`):

| Property | Purpose |
|----------|---------|
| `bDoesTrackActorsForGameManagement` | Master switch — disables collision component when `false` |
| `bCanSenseActors` | Whether sensing queries return results |
| `bCanTouchActors` | Whether touch interactions are permitted |

**Logging categories:** `Log_PhysicalSenseComponent`, `Log_PhysicalSenseComponent_Setup`, `Log_PhysicalSenseComponent_Runtime`

---

## Build Targets

All targets share settings via `UGEGameTarget.ApplySharedTargetSettings()`.

| Target file | Type | Notes |
|-------------|------|-------|
| `UGEGame` | Game | Base; `ShadowVariableWarningLevel = Error`; HTTPS cert validation in Shipping |
| `UGEGameSteam` | Game | Steam platform variant (extends `UGEGameTarget`) |
| `UGEGameEOS` | Game | EOS platform variant |
| `UGEGameSteamEOS` | Game | Steam + EOS combined |
| `UGEEditor` | Editor | Enables `RemoteSession` plugin for Unreal Remote 2 |
| `UGEClient` | Client | Client-only build |
| `UGEServer` | Server | `bUseChecksInShipping = true` |
| `UGEServerSteam` | Server | Steam dedicated server |
| `UGEServerEOS` | Server | EOS dedicated server |
| `UGEServerSteamEOS` | Server | Steam + EOS dedicated server |

Shared settings: logging enabled in Shipping, RHI resource tracking in Test, `ASSETREGISTRY_INDIRECT_ASSETDATA_POINTERS` in non-Editor builds, Game Feature plugin auto-configuration.

Additional plugin search paths: `AssetPlugins`, `CorePlugins`, `EditorPlugins`, `WidgetPlugins`, `SamplePlugins`.

---

## Branch Strategy

### Stable Branches
| Branch | Purpose |
|--------|---------|
| `master` | Stable production version |
| `EMPTY_BASE` | Clean base — branch out from here for new features |
| `config` | Configuration files |
| `build` | Latest build configuration |

### Development Branches
| Branch | Purpose |
|--------|---------|
| `dev` | Active development |
| `feature-all` | All feature branches merged for integration testing |
| `feature-editor-all` | All editor features merged |
| `llm-generated` | LLM-generated content |

### Plugin Feature Branches
| Branch | Plugin |
|--------|--------|
| `feature-asset-loader` | AssetLoader |
| `feature-input-system` | InputSystem |
| `feature-elemental-collection` | Elemental_Collection |
| `feature-core-collection` | Core_Collection |
| `feature-controllers` | Controllers |
| `feature-humanoids` | Humanoids |
| `feature-helicopters` | Helicopters |
| `feature-vehicles` | Vehicles |
| `feature-ships` | Ships |
| `feature-aircrafts` | Aircrafts |

### Game Feature Branches
| Branch | Feature |
|--------|---------|
| `feature-GF-humanoid-movement` | GameFeature_HumanoidMovement |
| `feature-GF-physical-awareness` | GameFeature_PhysicalAwareness |

### Editor Branches
| Branch | Purpose |
|--------|---------|
| `feature-editor-manager` | Main editor development branch |
| `feature-editor-entity-core` | Entity core editor and debugging |
| `feature-editor-entity-visualizer` | Entity visualizer and debugging |
