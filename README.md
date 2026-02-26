# Unified Game Equipment (UGE)

Unreal Engine **5.6** — Modular, plugin-based game framework built on Lyra foundations.
Provides a reusable equipment system for humanoids, helicopters, vehicles, ships, and aircraft with a unified controller, physics-driven flight, GAS integration, and data-driven configuration.

# Content Examples:
Content examples reside inside its own Project, without any codebase at all.
This gives me the freedom to work on the code without relying on specialized assets.
Github Repository: https://github.com/Held0fTheWelt/UGE_ContentExamples

# Architecture Overview

The project is structured in dependency layers. Lower layers have no knowledge of higher ones.

```
┌─────────────────────────────────────────────────────────┐
│  Game Features  AccoladeSystem · AnimationSystem ·      │
│                 CosmeticsSystem · EquipmentSystem ·     │
│                 ExperienceSystem · FeedbackSystem ·     │
│                 HeroSystem · HumanoidMovement ·         │
│                 Interact · InteractionSystem ·          │
│                 InventorySystem · PawnExtension ·       │
│                 PhysicalAwareness · Tournament          │
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

#### GameFeature_AccoladeSystem
Kill/streak/assist feedback system. Processes elimination and damage messages and displays data-driven accolade toasts in the HUD.

**Message processors** (placed as components on GameState):

| Class | Purpose |
|-------|---------|
| `UAssistProcessor` | Tracks damage per attacker; broadcasts `Assist.Message` to all non-killers when a target dies |
| `UElimChainProcessor` | **Abstract** — tracks rapid kill chains (time window: `ChainTimeLimit`, default 4.5 s); subclass in Blueprint and populate `ElimChainTags` |
| `UElimStreakProcessor` | **Abstract** — tracks kill streaks without own death; subclass in Blueprint and populate `ElimStreakTags` |

**Widget:** `UAccoladeHostWidget` (`UCommonUserWidget`) — listens for `Accolade.Message`, async-loads icon/sound from DataRegistry, calls BP events `CreateAccoladeWidget` / `DestroyAccoladeWidget`. `LocationName` tag controls which accolades are displayed per HUD instance.

**Data:** `FAccoladeDefinitionRow` (DataTable row: display name, sound, icon, duration, `LocationTag`, suppression rules) · `UAccoladeDefinition` (standalone DataAsset variant).

**Messages:**

| Tag | Direction | Struct |
|-----|-----------|--------|
| `WarCollection.Elimination.Message` | Incoming | `FVerbMessage` |
| `Core.Damage.Message` | Incoming | `FVerbMessage` |
| `Assist.Message` | Outgoing | `FVerbMessage` |
| `Accolade.Message` | Outgoing | `FNotificationMessage` |

Dependencies: `Elemental_MessageSystem`, `CommonUI`, `DataRegistry`, `AsyncMixin`.

---

#### GameFeature_AnimationSystem
Animation base system. Provides `UFeaturedAnimInstance` — a `UAnimInstance` subclass that automatically binds to the owning pawn's `UAbilitySystemComponent`, maps gameplay tags to AnimBP properties via `FGameplayTagBlueprintPropertyMap`, and computes ground distance each frame via `ICharacterGroundInfoProviderInterface`.

**Blueprint API:**

| Method | Purpose |
|--------|---------|
| `InitializeWithAbilitySystem(ASC)` | Binds the TagPropertyMap to the ASC |
| `HasGameplayTag(Tag)` | Returns true if the tag is present on the ASC |
| `IsAbilityActive(Tag)` | Checks whether the ability tag is in the owned tags |
| `GetAttributeValue(Tag)` | Returns an attribute value |

**Usage:** derive the AnimBP from `UFeaturedAnimInstance`; populate `GameplayTagPropertyMap` in Class Defaults to bind tags to float/bool/enum properties — no C++ required for new bindings.

**Placement:** set the AnimBP class on the Pawn to a `UFeaturedAnimInstance` subclass; implement `ICharacterGroundInfoProviderInterface` on a pawn component or the owner to supply `GroundDistance`.

Dependencies: `AnimationCore`, `GameplayAbilities`, `GameplayTags`, `Elemental_Interfaces`.

---

#### GameFeature_CosmeticsSystem
Cosmetics system for modularly swappable character parts (head, body, equipment pieces). The controller determines which parts are desired; the pawn spawns and manages the actual actor instances with replication.

**Components:**

| Class | Purpose |
|-------|---------|
| `UControllerComponent_CharacterParts` | `UControllerComponent` — holds the list of desired `FCharacterPart` entries for the possessed pawn |
| `UPawnComponent_CharacterParts` | `UActorComponent` — spawns/despawns cosmetic actors; replicates active part list via `FFastArraySerializer`; exposes `FSpawnedCharacterPartsChanged` delegate |

**Types:** `FCharacterPart` (actor class + optional socket) · `FCharacterPartHandle` (opaque handle) · `ECharacterCustomizationCollisionMode` (NoCollision / UseProfileFromPart).

**Animation support:** `FCosmeticAnimLayerSelectionSet` and `FCosmeticAnimBodyStyleSelectionSet` — tag-driven selection of anim layers and AnimBP types per body style.

**Placement:** `UControllerComponent_CharacterParts` on PlayerController · `UPawnComponent_CharacterParts` on Pawn.

Dependencies: `Engine`, `ModularGameplay`, `GameplayTags`, `Elemental_Interfaces`, `NetCore`.

---

#### GameFeature_EquipmentSystem
Equipment system. Manages equipped items on pawns (weapons, gadgets, etc.): spawns actors, grants GAS ability sets, and replicates state. `UQuickBarComponent` provides the controller with a slot-based selection interface.

**Core classes:**

| Class | Purpose |
|-------|---------|
| `UEquipmentDefinition` | **Abstract** `UObject` — defines equipment: `AbilitySetsToGrant`, `ActorsToSpawn` |
| `UEquipmentInstance` | `UObject` — replicated runtime instance; BP events `OnEquipped` / `OnUnequipped` |
| `UEquipmentManagerComponent` | `UPawnComponent` — manages equipment list with replication; equip/unequip + ability grant/revoke |
| `UQuickBarComponent` | `UControllerComponent` — slot-based quick bar; replicates active index |
| `UGameplayAbility_FromEquipment` | Base ability class for abilities granted by equipment; provides access to `UEquipmentInstance` and `UInventoryItemInstance` |

**Placement:** `UEquipmentManagerComponent` on Pawn · `UQuickBarComponent` on PlayerController · `UEquipmentDefinition` subclass DataAsset per equipment type.

**Messages:**

| Tag | Direction | Struct |
|-----|-----------|--------|
| `QuickBar.Message.SlotsChanged` | Outgoing | `FQuickBarSlotsChangedMessage` |
| `QuickBar.Message.ActiveIndexChanged` | Outgoing | `FQuickBarActiveIndexChangedMessage` |

Dependencies: `GameplayAbilities`, `Elemental_Interfaces`, `Elemental_MessageSystem`, `IrisCore`, `Niagara`.

---

#### GameFeature_ExperienceSystem
Experience loading system. Loads and activates a `UExperienceDefinition` at runtime: registers GameFeature plugins, activates ability sets, and configures the game session.

**Classes:**

| Class | Purpose |
|-------|---------|
| `UExperienceManagerComponent` | `UGameStateComponent` — orchestrates the experience loading state machine (`EExperienceLoadState`); activates/deactivates GameFeature plugins and ability sets; broadcasts `OnExperienceLoaded` |
| `UAsyncAction_ExperienceReady` | `UBlueprintAsyncActionBase` — BP node `WaitForExperienceReady`; fires `OnReady` once fully loaded (also fires retroactively if already loaded) |

**Placement:** `UExperienceManagerComponent` on GameState · `UExperienceDefinition` DataAsset configured via GameMode or Level Blueprint.

Dependencies: `GameFeatures`, `GameplayTags`, `CommonLoadingScreen`, `Elemental_Interfaces`, `CommonUser`, `Core_Data`.

---

#### GameFeature_FeedbackSystem
Feedback system. Provides two independent sub-systems: **Context Effects** (tag-driven, surface-aware VFX and audio triggered from animation notify states) and **Number Pops** (floating damage/healing numbers displayed via a controller component).

**Context Effects classes:**

| Class | Purpose |
|-------|---------|
| `IContextEffectsInterface` | Blueprintable interface — `AnimMotionEffect()` call from AnimNotify states |
| `UContextEffectComponent` | `UActorComponent`, implements `IContextEffectsInterface` — resolves physical surface to a context tag, queries libraries, spawns matching sounds and Niagara systems via `UContextEffectsSubsystem` |
| `UContextEffectsLibrary` | `UObject` DataAsset — `FContextEffects` entries (`EffectTag` + `Context` tags → soft refs to `USoundBase` / `UNiagaraSystem`); async-loads on demand; `ExactMatch` / `BestMatch` resolution |
| `UContextEffectsSubsystem` | `UWorldSubsystem` — per-actor library registry; `SpawnContextEffects()`, `GetContextFromSurfaceType()`, `LoadAndAddContextEffectsLibraries()` |
| `UContextEffectsSettings` | `UDeveloperSettings` — `SurfaceTypeToContextMap` (`EPhysicalSurface → FGameplayTag`) |

**Number Pop classes:**

| Class | Purpose |
|-------|---------|
| `UNumberPopComponent` | **Abstract** `UControllerComponent` — `AddNumberPop(FNumberPopRequest)`; subclass in Blueprint to implement the actual display |
| `FNumberPopRequest` | Struct — `WorldLocation`, `SourceTags`, `TargetTags`, `NumberToDisplay`, `bIsCriticalDamage` |
| `UDamagePopStyle` | `UDataAsset` — style per damage type: `DisplayText`, `MatchPattern` (`FGameplayTagQuery`), `Color`, `CriticalColor`, `TextMesh` |

**Placement:** `UContextEffectComponent` on Pawn · Blueprint subclass of `UNumberPopComponent` on PlayerController · `UContextEffectsLibrary` and `UDamagePopStyle` DataAssets · `UContextEffectsSettings` in Project Settings.

Dependencies: `ModularGameplay`, `GameplayTags`, `Niagara`, `AudioMixer`, `PhysicsCore`, `DeveloperSettings`, `Elemental_Interfaces`.

---

#### GameFeature_HeroSystem
Pawn input and camera coordinator for player-controlled characters. `UHeroComponent` hooks into the `IGameFrameworkInitStateInterface` protocol and handles two responsibilities: forwarding input tag events to the ASC and managing ability-driven camera mode overrides.

**InitState transitions** (depends on `UPawnExtensionComponent`):

| Transition | Action |
|------------|--------|
| `DataAvailable → DataInitialized` | Calls `IPawnExtensionComponentInterface::Execute_InitializeAbilitySystem()`; broadcasts `NAME_BindInputsNow` extension event to Pawn and PlayerController |
| `DataInitialized → GameplayReady` | Pawn is fully ready for gameplay |

**Key methods:** `SetAbilityCameraMode()` / `ClearAbilityCameraMode()` — override active camera mode from within an ability · `Input_AbilityInputTagPressed/Released()` — forwards events to `IAbilitySystemInputInterface` on the ASC owner.

**Placement:** `UHeroComponent` on Pawn (requires `UPawnExtensionComponent` and an ASC on PlayerState).

Dependencies: `ModularGameplay`, `GameplayAbilities`, `GameplayTags`, `Elemental_Interfaces`, `Elemental_Common`.

---

#### GameFeature_HumanoidMovement
Adds humanoid movement mechanics (walking, running, jumping animations and input binding) to `AHumanoidCharacter` via the modular Game Feature action system.
Module: `GameFeature_HumanoidMovementRuntime` (Runtime, Default).
State: **Active by default** (`BuiltInInitialFeatureState: Active`).

---

#### GameFeature_Interact
Proximity-based interaction system. Adds collision-driven interaction zones, UI feedback, and pawn-swapping support to any actor.

Modules:

| Module | Type | Purpose |
|--------|------|---------|
| `GameFeature_InteractRuntime` | Runtime | Interaction logic, collision, widgets |
| `GameFeature_InteractSetup` | Editor/Setup | Settings and editor customization |

**Core component — `UInteractCollisionComponent`:**
- Manages one or more box collision components (`QueryOnly`, `ECC_GameTraceChannel2`) that detect pawn overlaps
- On overlap begin: registers with the player controller (`IRegisterComponentInterface`), shows the interaction widget, and registers the object with the pawn's sense systems (touch + visual)
- On overlap end: unregisters from the controller, removes the widget, and forgets the object from sense systems
- On Interact input (`IInteractControlInterface::Interact`): broadcasts `FOnInteractWithPawn` to the owner actor
- Configurable interaction limit (global default via `UGameFeature_Interact_Settings`, per-component override)
- Collision shape loaded from `UCollisionBoxConfigDataAsset`; supports 90° rotation variants

**Widget system:**
- `UInteractUserWidgetBase` — base widget class implementing `IInteractWidgetInterface` (`SetImage`, `SetText`)
- Widget class loaded from `UWidgetDefinitionDataAsset`; icon texture from `UActorUIImageDataAsset`
- Added/removed from viewport dynamically on proximity enter/exit

**Pawn-swapping integration (`InteractManageGameInstanceSubsystem`):**
- Enter vehicle: unpossesses current pawn → possesses vehicle pawn
- Exit vehicle: spawns default pawn at vehicle location → unpossesses vehicle → possesses new pawn
- Dummy-to-real replacement: swaps `IInteractDummyInterface` placeholder actors with defined actor types

**Settings (`UGameFeature_Interact_Settings`):**
- Global debug mode for collision query visualization
- Configurable default interaction limit
- Overridable via `DefaultGame.ini`

**Logging categories:** `Log_Interact`, `Log_Interact_Debug`, `Log_Interact_Setup`, `Log_Interact_Widget`, `Log_Interact_Class`

---

#### GameFeature_InteractionSystem
GAS-based interaction system. Detects `IInteractableTarget` objects near the player via ability tasks (line trace or overlap), dynamically grants interaction abilities, and communicates progress via the `GameplayMessageSubsystem`.

**Classes:**

| Class | Purpose |
|-------|---------|
| `UGameplayAbility_Interact` | **Abstract** `UGameplayAbility` — base for interaction abilities; holds `CurrentOptions` (`TArray<FInteractionOption>`); calls `TriggerInteraction()`; subclass in Blueprint |
| `UAbilityTask_GrantNearbyInteraction` | Grants interaction abilities to nearby `IInteractableTarget` objects (overlap-based); cleaned up when task ends |
| `UAbilityTask_WaitForInteractableTargets_SingleLineTrace` | Monitors interactable targets via single line trace (configurable channel, length, interval) |
| `UInteractionStatics` | `UBlueprintFunctionLibrary` — `GetInteractableTargetsFromActor()`, `AppendInteractableTargetsFromHitResult()` |

**Placement:** Blueprint subclass of `UGameplayAbility_Interact` as a startup ability on the Pawn's ASC · `IInteractableTarget` implemented on interactable world actors.

**Messages:**

| Tag | Direction | Struct |
|-----|-----------|--------|
| `Interaction.Duration.Message` | Outgoing | `FInteractionDurationMessage` (progress 0..1 + target) |

Dependencies: `GameplayAbilities`, `GameplayTasks`, `Core_Interfaces`, `Core_Interact`, `Elemental_Interfaces`, `UMG`.

---

#### GameFeature_InventorySystem
Inventory system. Manages item instances on pawns with replication, supports fragment-based item extensions (equippable, stats, icons), and provides a world pickup component integrated with the interaction system.

**Core classes:**

| Class | Purpose |
|-------|---------|
| `UInventoryItemDefinition` | **Abstract** `UObject` — defines an item type via a `Fragments` array |
| `UInventoryItemInstance` | `UObject` — replicated runtime instance; holds a `StatTagStack` map |
| `UInventoryManagerComponent` | `UPawnComponent` — manages item list with `FFastArraySerializer` replication |
| `UWorldPickupComponent` | `UCombinedBundleActorComponent` + `IInteractableTarget` — on pickup calls `UInventoryManagerComponent::AddItemDefinitionTyped()` on the instigator |
| `UInventorySubsystem` | `UWorldSubsystem` — tag-based inventory lookup by actor |
| `UInventoryFunctionLibrary` | `UBlueprintFunctionLibrary` — `FindFragmentByClass()`, fragment data access |

**Fragments:**

| Class | Purpose |
|-------|---------|
| `UInventoryFragment_EquippableItem` | Links item to the EquipmentSystem via slot tag + `UEquipmentDefinition` |
| `UInventoryFragment_SetStats` | Initial stat values (tag → int32 map) |
| `UInventoryFragment_PickupIcon` | Pickup texture for world display |
| `UInventoryFragment_QuickBarIcon` | QuickBar slot icon texture |

**Placement:** `UInventoryManagerComponent` on Pawn · `UWorldPickupComponent` on world actors · `UInventoryItemDefinition` subclass DataAssets with fragment configuration.

**Messages:**

| Tag | Direction | Struct |
|-----|-----------|--------|
| `InventoryMessageStackChanged` | Outgoing | `FInventoryChangeMessage` |

Dependencies: `Core_Interfaces`, `Core_Interact`, `Elemental_Interfaces`, `Elemental_Player`, `GameplayAbilities`, `GameplayMessageRuntime`, `NetCore`.

---

#### GameFeature_PawnExtension
Pawn initialization coordinator. Ensures all pawn subsystems (AbilitySystemComponent, input, PawnData) are initialized in the correct order — including async loading and late-joining scenarios. Implements the `IGameFrameworkInitStateInterface` protocol.

**`UPawnExtensionComponent`** (`UPawnComponent` + `IGameFrameworkInitStateInterface` + `IPawnExtensionComponentInterface`): central coordinator for pawn initialization; manages `PawnData`, binds the ASC, processes input configuration; exposes `OnAbilitySystemInitialized` / `OnAbilitySystemUninitialized` delegates.

**InitState protocol** (four ordered states from `GameplayTags_Core`):

| Tag | Meaning |
|-----|---------|
| `InitState.Spawned` | Actor spawned, basic setup complete |
| `InitState.DataAvailable` | PawnData and dependencies present |
| `InitState.DataInitialized` | ASC and all systems initialized |
| `InitState.GameplayReady` | Pawn fully ready for gameplay |

Other components (input, ability grants, cosmetics) listen for these states and initialize only once their prerequisites are met.

**Placement:** `UPawnExtensionComponent` on Pawn (must be early in the component stack).

Dependencies: `ModularGameplay`, `GameplayAbilities`, `GameplayTags`, `Core_Structure`, `Elemental_Classes`, `Elemental_Interfaces`, `Elemental_Structure`.

---

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

#### GameFeature_Tournament
Tournament system. Currently implemented as a module stub — contains no gameplay classes yet. Intended for tournament management logic (brackets, rounds, score tracking).

**Planned responsibilities:**
- Tournament bracket management (single/double elimination, round robin)
- Round and match state tracking
- Score and ranking persistence

Status: **Pending implementation.**

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
| `feature-GF-AccoladeSystem` | GameFeature_AccoladeSystem |
| `feature-GF-AnimationSystem` | GameFeature_AnimationSystem |
| `feature-GF-CosmeticsSystem` | GameFeature_CosmeticsSystem |
| `feature-GF-EquipmentSystem` | GameFeature_EquipmentSystem |
| `feature-GF-ExperienceSystem` | GameFeature_ExperienceSystem |
| `feature-GF-FeedbackSystem` | GameFeature_FeedbackSystem |
| `feature-GF-HeroSystem` | GameFeature_HeroSystem |
| `feature-GF-humanoid-movement` | GameFeature_HumanoidMovement |
| `feature-GF-interact` | GameFeature_Interact |
| `feature-GF-InteractionSystem` | GameFeature_InteractionSystem |
| `feature-GF-InventorySystem` | GameFeature_InventorySystem |
| `feature-GF-PawnExtension` | GameFeature_PawnExtension |
| `feature-GF-physical-awareness` | GameFeature_PhysicalAwareness |
| `feature-GF-Tournament` | GameFeature_Tournament |

### Editor Branches
| Branch | Purpose |
|--------|---------|
| `feature-editor-manager` | Main editor development branch |
| `feature-editor-entity-core` | Entity core editor and debugging |
| `feature-editor-entity-visualizer` | Entity visualizer and debugging |
