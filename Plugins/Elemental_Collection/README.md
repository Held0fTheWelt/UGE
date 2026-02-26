# Elemental_Collection

Core implementation plugin for the UGE project. Provides the foundational game framework built on Unreal's Modular Gameplay and Game Features system: actor base classes, player/team management, ability system integration, a comprehensive interface library, data-driven entity configuration, UI management, and message routing.

---

## Quick Start

For framework assembly in the UE Editor:
→ **`Docs/References/Ref_LyraFramework_UEAssembly.md`**

Short version: GameInstance = `UGameInstance_Core`, WorldSettings = `ACoreWorldSettings`,
set `DefaultGameplayExperience` DataAsset, add `UExperienceManagerComponent` to GameState.

---

## Module Overview

| Module | Type | Purpose |
|---|---|---|
| `Elemental_Classes` | Runtime | Modular base actors, game state/player state/controller, components |
| `Elemental_Common` | Runtime | UI framework, game instance, async actions, local player, audio provider |
| `Elemental_Data` | Runtime | Data asset library for entity configuration (mesh, camera, vehicle, audio…) |
| `Elemental_Editor` | Editor | Property type customizations for Inspector |
| `Elemental_Interfaces` | Runtime | ~80 interfaces decoupling all system contracts |
| `Elemental_Logging` | Runtime | Log category declarations |
| `Elemental_Network` | Runtime | Compressed network structs (acceleration, movement) |
| `Elemental_Settings` | Runtime | Local (machine) and shared (cloud) player settings base classes |
| `Elemental_Player` | Runtime | Player state with GAS + teams, local player, spawn management, squad subsystem |
| `Elemental_Teams` | Runtime | Team info actors, display assets, team subsystem, team creation component |
| `Elemental_MessageSystem` | Runtime | Gameplay message structs, replication, and message processor base |
| `Elemental_Structure` | Runtime | Enums, structs, gameplay tag stack replication system |

---

## Plugin Dependencies

GameplayAbilities · GameplayMessageRouter · ModularGameplay · GameFeatures · EnhancedInput · ChaosVehiclesPlugin · SignificanceManager · ReplicationGraph · CommonUI · CommonUser · AssetLoader · LoggingMacros · GameSubtitles

---

## Architecture

The plugin follows a Lyra-inspired modular game framework pattern:

```
Game Mode / Game State (AModularGameState)
  └── GameStateComponents (loaded by Experience)
        ├── UTeamCreationComponent   → creates ATeamPublicInfo / ATeamPrivateInfo
        ├── UBotCreationComponent    → spawns AI controllers
        └── UPlayerSpawningGameStateComponent → manages APlayerStart_Base claiming

Player Controller (AModularPlayerController / APlayerController_Elemental)
  └── APlayerState_Elemental
        ├── AbilitySystemComponent
        ├── FGameplayTagStackContainer (StatTags)
        ├── ITeamAgentInterface  →  UTeamSubsystem
        └── Squad ID             →  USquadSubsystem

Local Pawn
  └── IPawnExtensionComponentInterface  (init state machine)
        ├── InitState_Spawned
        ├── InitState_DataAvailable
        ├── InitState_DataInitialized
        └── InitState_GameplayReady    (provided by UElementalInitStateTagSubsystem)
```

**Experience loading** is the trigger for most runtime setup. Components listen via `IExperienceManagerInterface::CallOrRegister_OnExperienceLoaded`. The experience system itself is expected to be provided by a separate game feature plugin; this plugin only defines the interfaces and listener contracts.

**Interface-first design**: Almost nothing in this plugin refers to concrete implementations. Actors, components, and subsystems communicate through the interfaces in `Elemental_Interfaces`, which keeps game feature plugins and other modules decoupled.

---

## Elemental_Classes

Base actor classes for the modular game framework. All classes call engine extension points (`PreInitializeComponents`, `BeginPlay`, `EndPlay`) so game feature plugin components receive lifecycle events.

### States & Controllers

| Class | Parent | Description |
|---|---|---|
| `AModularGameStateBase` | `AGameStateBase` | Extensible base game state (no teams/players) |
| `AModularGameState` | `AGameState` | Full game state with match start hook |
| `AModularPlayerState` | `APlayerState` | Extensible player state |
| `AModularPlayerController` | `ACommonPlayerController` | Extensible player controller |
| `AModularAIController` | `AAIController` | Extensible AI controller |

### Components

| Class | Parent | Description |
|---|---|---|
| `UPlayerSpawningManagerComponent` | `UGameStateComponent` | Caches `APlayerStart_Base` actors, selects spawn points, notifies on respawn |
| `UBotCreationComponent` | `UGameStateComponent` | Abstract; spawns/removes AI controllers on experience load |
| `UTrafficPathComponent` | `USplineComponent` | Spline path for AI/vehicle traffic |
| `UDefinedPointLightComponent` | `UPointLightComponent` | Extensible point light stub |
| `UDefinedSpotLightComponent` | `USpotLightComponent` | Extensible spot light stub |
| `UNoTouchDynamicMeshComponent` | `UDynamicMeshComponent` | Dynamic mesh with `INoTouchCollisionInterface` |

### Actors & Subsystems

| Class | Parent | Description |
|---|---|---|
| `APlayerStart_Base` | `APlayerStart` | Tag-based player start with controller claiming |
| `USpawnInstanceSubsystem` | `UGameInstanceSubsystem` | Centralized actor and pawn spawning |

---

## Elemental_Common

UI framework, game instance, async Blueprint actions, and auxiliary subsystems.

### UI System

```
UGameUIManagerSubsystem  (GameInstance)
  └── UGameUIPolicy  (per-player strategy)
        └── UPrimaryGameLayout  (root widget)
              ├── Layer: FGameplayTag → UCommonActivatableWidgetContainerBase
              └── Layer: ...
```

Layers are registered in `UPrimaryGameLayout::RegisterLayer(FGameplayTag, UCommonActivatableWidgetContainerBase*)`. Widgets are pushed with `PushWidgetToLayerStack<T>()` or the async variant `PushWidgetToLayerStackAsync<T>()`.

### Async Blueprint Actions

| Class | Description |
|---|---|
| `UAsyncAction_CreateWidgetAsync` | Streams a widget class, then instantiates it; optionally suspends input |
| `UAsyncAction_PushContentToLayerForPlayer` | Streams an activatable widget and pushes it to a UI layer |
| `UAsyncAction_ShowConfirmation` | Shows a Yes/No/Cancel dialog, returns `ECommonMessagingResult` |

### Key Classes

| Class | Description |
|---|---|
| `ACommonPlayerController` | Base player controller with pawn possession/unpossession hooks |
| `UCommonLocalPlayer` | Local player with `OnPlayerControllerSet/StateSet/PawnSet` delegates |
| `UCommonGameInstance` | Session and user lifecycle management |
| `UCommonMessagingSubsystem` | Per-local-player dialog subsystem |
| `UCommonUIExtensions` | Blueprint library: input type query, layer push/pop, input suspend |
| `UElementalAudioProviderSubsystem` | Register/unregister `IAudioSystemInterface` implementations at runtime |
| `UElementalInitStateTagSubsystem` | Provides the four init state `FGameplayTag` values and pawn extension feature name |
| `UCommonPlayerInputKey` | Widget that displays the current keybind/button icon for an action, with hold-progress rendering |

### Confirmation Dialogs

```cpp
// Async (Blueprint-friendly)
UAsyncAction_ShowConfirmation::ShowConfirmationYesNo(this, Title, Body)
    ->OnResult.AddDynamic(this, &UMyClass::OnResult);

// Direct (C++)
UCommonMessagingSubsystem* Messaging = LocalPlayer->GetSubsystem<UCommonMessagingSubsystem>();
Messaging->ShowConfirmation(Descriptor, FCommonMessagingResultDelegate::CreateUObject(...));
```

---

## Elemental_Player

Player-specific classes built on `Elemental_Classes`.

### `APlayerState_Elemental`
Extends `AModularPlayerState` with:
- **GAS**: implements `IAbilitySystemInterface`; holds `AbilitySystemComponent`, `HealthSet`, `CombatSet`
- **Teams**: implements `ITeamAgentInterface`; replicates `MyTeamID`
- **Squads**: replicates `MySquadID`; notifies `USquadSubsystem` on change
- **Stat Tags**: replicated `FGameplayTagStackContainer` for tag-based player stats
- **View Rotation**: replicated for spectator support

Key methods:
```cpp
void AddStatTagStack(FGameplayTag Tag, int32 Count);          // AuthorityOnly
void RemoveStatTagStack(FGameplayTag Tag, int32 Count);       // AuthorityOnly
int32 GetStatTagStackCount(FGameplayTag Tag) const;
template<T> const T* GetPawnData() const;
void ClientBroadcastMessage(const FVerbMessage Message);      // Client RPC
```

### `ULocalPlayer_Elemental`
Extends `UCommonLocalPlayer` + `ITeamAgentInterface`. Loads shared settings from disk; returns `nullptr` for local settings (subclass responsibility).

### `UPlayerSpawningGameStateComponent`
Tick-based component on the game state that calls `ChoosePlayerStart` and `FinishRestartPlayer`. Override `OnChoosePlayerStart` (C++) or `K2_OnFinishRestartPlayer` (Blueprint) for custom spawn logic.

### `USquadSubsystem`
World subsystem notified when `APlayerState_Elemental::MySquadID` changes. Override `OnPlayerSquadIdChanged` (BlueprintNativeEvent) to react.

---

## Elemental_Teams

Full team management system.

### Team Info Actors (server-authoritative)

| Class | Replication | Description |
|---|---|---|
| `ATeamInfoBase` | `bAlwaysRelevant`, priority 3 | Abstract base with `FGameplayTagStackContainer TeamTags` |
| `ATeamPublicInfo` | `COND_InitialOnly` | Holds `UTeamDisplayAsset`; replicated to all clients |
| `ATeamPrivateInfo` | Authority only | Team data invisible to other teams |

### `UTeamDisplayAsset`
Data asset with `TMap<FName, float>` scalar, `TMap<FName, FLinearColor>` color, and `TMap<FName, UTexture*>` texture parameters. Apply to actors/components:
```cpp
DisplayAsset->ApplyToActor(TargetActor, /*bIncludeChildActors=*/true);
DisplayAsset->ApplyToMaterial(MaterialInstance);
DisplayAsset->ApplyToMeshComponent(MeshComp);
```

### `UTeamSubsystem`
Central world subsystem for all team queries:
```cpp
// Team lookup
bool bIsPartOfTeam; int32 TeamId;
Subsystem->FindTeamFromActor(Actor, bIsPartOfTeam, TeamId);

// Damage check
bool bCanDamage = Subsystem->CanCauseDamage(Instigator, Target);

// Comparison (ExpandEnumAsExecs)
ETeamComparison Result = Subsystem->CompareTeams(A, B, TeamIdA, TeamIdB);

// Tag stacks on teams
Subsystem->AddTeamTagStack(TeamId, Tag, Count);
```

### `UTeamCreationComponent`
Game state component. Configured with `TeamsToCreate (TMap<uint8, UTeamDisplayAsset*>)`. Creates teams and assigns players to least-populated team on experience load. Override `ServerChooseTeamForPlayer` for custom assignment logic.

---

## Elemental_MessageSystem

### `FVerbMessage`
Generic gameplay event message (defined in `Elemental_Structure`):

| Field | Type | Description |
|---|---|---|
| `Verb` | `FGameplayTag` | Action tag (e.g. `"GameplayEvent.Death"`) |
| `Instigator` | `UObject*` | Source |
| `Target` | `UObject*` | Target |
| `InstigatorTags` | `FGameplayTagContainer` | Source context tags |
| `TargetTags` | `FGameplayTagContainer` | Target context tags |
| `ContextTags` | `FGameplayTagContainer` | Additional context |
| `Magnitude` | `double` | Scalar payload (default 1.0) |

### `FNotificationMessage`
Transient HUD notification (elimination feed, pickups, etc.). Broadcast on `TAG_AddNotification_Message` (`"AddNotification.Message"`). Carries a target player (or `nullptr` for all), display text, style tag, and optional payload object.

### `FVerbMessageReplication`
Fast-array serializer container for replicating verb messages on `APlayerState_Elemental`. Call `AddMessage(FVerbMessage)` on authority; clients receive broadcast via `GameplayMessageRouter`.

### `UGameplayMessageProcessor`
Abstract `UActorComponent` base for server-side message observers. Spawned once per server (not per player). Override `StartListening()` and `StopListening()`, use `AddListenerHandle()` to register `FGameplayMessageListenerHandle` instances.

### Helpers
`UVerbMessageHelpers` — static Blueprint library: extract `APlayerState`/`APlayerController` from `UObject`, convert between `FVerbMessage` and `FGameplayCueParameters`.

---

## Elemental_Interfaces

~80 interfaces providing all system contracts. Concrete implementations live in game feature plugins.

### Control Interfaces (`Interfaces/Control/`)

One interface per controllable entity type — implemented by Pawn/Character classes and called by the player controller after routing through the input system:

| Interface | Entity | Key Methods |
|---|---|---|
| `IHumanoidControlInterface` | Biped | `Move`, `Look`, `Jump`, `Run` |
| `IVehicleControlInterface` | Ground vehicle | `Steering`, `Throttle`, `Brake`, `Handbrake` |
| `IAircraftControlInterface` | Fixed-wing | `StickInput`, `RudderInput`, `FlapsInput`, `BrakeKey` |
| `IHelicopterControlInterface` | Rotorcraft | `CyclicInput`, `CollectiveInput`, `PedalsInput`, `SASToggle` |
| `IShipControlInterface` | Watercraft | `HelmInput`, `ThrottleInput`, `BowThrusterInput`, `AnchorKey` |
| `ITurretControlInterface` | Turret | `Rotate` |
| `ICameraControlInterface` | Camera | `Look`, `ToggleCamera`, `ZoomCamera` |
| `IEngineControlInterface` | Engine | `StartEngineSwitch`, delegates for active state |
| `IEngineInputControlInterface` | Engine input | `Throttle` |
| `IInteractControlInterface` | Interaction | `Interact`, `StopInteracting` |
| `IPawnControlInterface` | Generic pawn | `GetPawnControlType`, `InteractOnPawn` |

### Selected Interfaces by Domain

**Ability System**

| Interface | Key Methods |
|---|---|
| `IAbilitySystemInputInterface` | `AbilityInputTagPressed/Released`, `ProcessAbilityInput`, `TryActivateAbilitiesOnSpawn` |
| `IAbilitySetInterface` | `GiveToAbilitySystem`, `TakeFromAbilitySystem`; `FAbilitySetGrantedHandles` tracks granted handles |
| `IAbilitySystemTagRelationshipReceiverInterface` | `SetTagRelationshipMapping` |

**Experience**

| Interface | Key Methods |
|---|---|
| `IExperienceManagerInterface` | `SetCurrentExperience`, `CallOrRegister_OnExperienceLoaded`, `IsExperienceLoaded`, `GetCurrentExperience` |
| `IExperienceLoadedListenerInterface` | `OnExperienceLoaded(const UObject* Experience)` |

**Health / Death**

| Interface | Key Methods |
|---|---|
| `IDeathExecutionInterface` | `StartDeath`, `FinishDeath`, `GetDeathState` (`EDeathExecutionState`) |
| `IDeathEventInterface` | `OnDeathStarted(AActor*)`, `OnDeathFinished(AActor*)` |
| `IHealthComponentInterface` | `GetHealth`, `DamageSelfDestruct` |

**Inventory / Equipment**

| Interface | Key Methods |
|---|---|
| `IInventoryManagerInterface` | `AddItemDefinition`, `GetInventoryItems`, `GetTotalItemCountByDefinition` |
| `IInventoryItemInstanceInterface` | `GetItemDefinitionClass`, `FindFragmentByClass` |
| `IEquipmentManagerInterface` | `GetEquipmentManagerComponent` |
| `IEquipmentDefinitionInterface` | `GetSlotTag` |

**Radar / Targeting**

| Interface | Key Methods |
|---|---|
| `IRadarDetectedInterface` | `GetRadarIdentifier`, `GetRadarReadableName` |
| `IRadarDetectingInterface` | `NotifyAttack`, `NotifyAttackOver`, `NotifyDeath` |
| `IFireWeaponInterface` | `GetProjectileSpeed`, `GetRelativeSocketLocation`, delegates for bullet speed and socket location |

**Scene / Replication**

| Interface | Key Methods |
|---|---|
| `IHumanoidReplicationInterface` | `UpdateSharedReplication`, `SetReplicatedAcceleration` |
| `ISceneComponentCatchInterface` | `GetSceneComponent(FName)` |
| `ISceneHierarchyInterface` | `GetSceneComponents()` → `TMap<FName, USceneComponent*>` |
| `IPossessedByControllerInterface` | `ControllerPossessesPawn`, `GetOnPawnPossessedByController` delegate |

**Audio**

| Interface | Key Methods |
|---|---|
| `IAudioSystemInterface` | `ApplyDynamicRangeEffectsChains`, `SetLoadingScreenMixActive`, volume control bus getters |
| `IAudioMixEffectsInterface` | `ApplyDynamicRangeEffectsChains`, `SetLoadingScreenMixActive` |
| `IAudioVolumeBusesProviderInterface` | Per-category control bus getters (Overall, Music, SFX, Dialogue, VoiceChat) |

**Registries (C++, no UInterface)**

| Registry | Purpose |
|---|---|
| `GameFeatureEvents::NAME_AbilityReady` | Event name fired when ability system is ready |
| `InteractionCollisionChannelRegistry` | Set/get the collision channel used for interaction traces |
| `IDevelopmentStaticsProvider` | Editor utility: `GetPlayInEditorAuthorityWorld`, `FindClassByShortName` |

---

## Elemental_Data

Data asset library. All concrete assets extend `UPreLoadingDataAsset` and are consumed by the AssetLoader plugin's loading pipeline. Each asset type implements `GetAssetType_Implementation()` (returns a unique `FPrimaryAssetType`) and optionally `GetAssetClasses_Implementation()` / `GetAssetObjects_Implementation()` for soft references to load.

| Asset | Configuration for |
|---|---|
| `UCameraSetupDataAsset` | SpringArm/camera per `ECameraViewType`; post-process; blend settings |
| `USkeletalMeshSetupDataAsset` | Skeletal mesh + anim instance |
| `UStaticMeshSetupDataAsset` | Static mesh |
| `UMeshCollectionSetupDataAsset` | Body + collection of `FMeshCollectionItem` (physics, collision, materials) |
| `UStaticMeshCollectionSetupDataAsset` | Body + collection of `FStaticMeshCollectionItem` |
| `UChaosVehicleSetupDataAsset` / `UElementalChaosVehicleSetupDataAsset` | Full Chaos vehicle: wheels, engine, differential, transmission, steering, aerodynamics, thrusters, stability |
| `UArrowComponentDefinitionDataAsset` | Named arrow components with transform, color, size |
| `UArrowComponentSetupDataAsset` | Display settings per `EArrowComponentType` |
| `UBarrelConfigDataAsset` | Weapon barrel mesh + fire socket name |
| `UCollisionBoxConfigDataAsset` | Simple/centered collision box flags |
| `UCameraSetupDataAsset` | Spring arm + view type configuration table |
| `USpawnPointDefinitionDataAsset` | Spawn point transforms grouped by `ESpawnPointType` |
| `USceneComponentCatchConfigDataAsset` | Named soft class refs for dynamic component lookup |
| `UActorUIImageDataAsset` | HUD image/widget attachment config |
| `UWidgetDefinitionDataAsset` | Soft class ref to a `UUserWidget` |
| `UWidgetComponentSetupDataAsset` | Widget component scale and health-bar height |
| `UTexture2DCollectionDataAsset` | Array of soft `UTexture2D` references |
| `UAnimatedTrackDataAsset` | Material scalar parameter name + target material indices |
| `UVehiclePhysicsSetupDataAsset` | Named vehicle physics setups |
| `UMainMeshRelativeTranslationDataAsset` | Relative transform for main mesh |
| `UActorDebugSettingsDataAsset` | Debug display settings (inherits `UInformativePrimaryDataAsset`) |

---

## Elemental_Structure

Foundation types shared across all modules.

### Gameplay Tag Stack

```cpp
// Replicated container — used on APlayerState_Elemental and ATeamInfoBase
FGameplayTagStackContainer StatTags;

StatTags.AddStack(Tag, Count);
StatTags.RemoveStack(Tag, Count);
int32 Count = StatTags.GetStackCount(Tag);
bool bHas  = StatTags.ContainsTag(Tag);
```

Uses `FFastArraySerializer` for efficient delta replication.

### Configuration Structs

| Struct | Purpose |
|---|---|
| `FViewTypeConfiguration` | Data-table row: full spring arm + camera config per `ECameraViewType` |
| `FMeshCollectionItem` | Mesh component with full physics, collision, and material configuration |
| `FStaticMeshCollectionItem` | Same as above for static meshes |
| `FAudioComponentSetup` | Audio component with fade, volume, pitch, looping, attenuation |
| `FCollisionSetup` | Shape-conditional collision config (sphere/box/capsule dimensions) |
| `FArrowComponentDefinition` | Named debug arrow with transform, color, size, length |
| `FSpawnPointsCollection` | `ESpawnPointType` + array of `FTransform` spawn points |
| `FWheelGroup` | Array of wheel indices for vehicle wheel grouping |
| `FVerbMessage` | Generic event message (see Elemental_MessageSystem) |
| `FInformationSet` | Category/name/description + typed value array (for debug/logging) |

### Key Enumerations

| Enum | Values (abbreviated) |
|---|---|
| `EPawnControlType` | NONE, HUMANOID, VEHICLE, AIRCRAFT, HELICOPTER, TURRET, SHIP |
| `ECameraViewType` | NONE, FIRST, THIRD, TOP, ABOVE, PANORAMIC, COCKPIT, DOOR_LEFT/RIGHT, … |
| `EArrowComponentType` | NONE, FORWARD, FORCE, VELOCITY, RESULTING_FORCE, DRAG |
| `EViewMode` | NONE, FIRST_PERSON, THIRD_PERSON, TOP_DOWN |
| `ESenseType` | NONE, VISUAL, AUDITIVE, TOUCH |
| `ETargetMode` | NONE, DIRECT, BALLISTIC_HIGH, BALLISTIC_LOW |
| `EUnitOfSpeed` | NONE, KNOTS, MPH, KPH |
| `ERuntimeMode` | OFF, ON, NEXT, PREVIOUS, RANDOM, EXACT |
| `ELightType` | NONE, DIRECTIONAL, SPOT, POINT |
| `ECollisionType` | NONE, SPHERE, BOX, CAPSULE, HULL, MESH, COMPOUND |
| `EPlayerConnectionType` | Player, LiveSpectator, ReplaySpectator, InactivePlayer |
| `EElementalSoundType` | NONE, INTERIOR, EXTERIOR, SIGNAL, UI |

---

## Elemental_Network

Two compressed structs for bandwidth-efficient replication:

**`FReplicatedAcceleration`** — encodes XY direction as a quantized angle (`uint8`) + XY magnitude (`uint8`) + Z component (`int8`). Used by `IHumanoidReplicationInterface::SetReplicatedAcceleration`.

**`FSharedRepMovement`** — wraps `FRepMovement` with movement mode, jump-force state, and crouch state. Filled via `FillForCharacter(ACharacter*)`. Implements `NetSerialize` with `WithNetSerializer = true, WithNetSharedSerialization = true`.

---

## Elemental_Settings

Base classes for the settings system — implementations are intentionally empty (filled in by game code or subclasses):

| Class | Parent | Scope |
|---|---|---|
| `UElementalSettings_Local` | `UGameUserSettings` | Machine-specific (graphics, bindings) |
| `UElementalSettings_Shared` | `ULocalPlayerSaveGame` | Cloud-safe (color blind, haptics, audio, sensitivity) |

`UElementalSettings_Shared` defines supporting enums: `EColorBlindMode`, `ELyraAllowBackgroundAudioSetting`, `ELyraGamepadSensitivity`.

`ULocalPlayer_Elemental` (in `Elemental_Player`) calls `GetSharedSettings()` and `LoadSharedSettingsFromDisk()`; the base implementation returns `nullptr` — subclass `ULocalPlayer_Elemental` to wire in a concrete settings object.

---

## Elemental_Editor

Registers five property type customizations at `PostEngineInit`:

| Struct | Customization | Effect |
|---|---|---|
| `FInformativeAssetClassRef` | `FInformativeAssetClassRefCustomization` | Dropdown of all concrete `UInformativePrimaryDataAsset` subclasses, filtered by unique `AssetTypeName` |
| `FViewTypeConfiguration` (in array) | `FViewTypeConfigurationCustomization` | Uses `ECameraViewType` name as array element header instead of `"Element [N]"` |
| `FActorAndComponentSelector` | `FActorAndComponentSelectorCustomization` | Hides irrelevant fields based on parent `EAssetDefinitionType` (Actor/Component) |
| `FDefiningContainerInformation` | `FDefiningContainerInformationCustomization` | Shows `DefiningAssetName` as header label |

`FAllowedClassFilter` — class viewer filter that restricts pickers to `AActor` or `UActorComponent` subclasses depending on `ECustomAssetDefinitionType`.
