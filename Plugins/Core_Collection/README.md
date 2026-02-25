# Core_Collection Plugin

A comprehensive game framework plugin providing the full gameplay stack for the UGE project — from foundational data types and settings up to ability systems, replication graphs, game feature actions, and editor tooling. Built on Unreal Engine 5.6's Lyra architecture and extended for a multi-entity (humanoid, vehicle, helicopter, aircraft, turret) game.

---

## Module Overview

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         EDITOR TOOLS                                    │
│  Core_Collection_Editor  (editor mode, validators, property viewers)    │
└───────────────────────────────┬─────────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────────┐
│                    INFRASTRUCTURE & TOOLING                             │
│  Core_HotfixSystem  │  Core_Replay  │  Core_Tests  │  Core_Development  │
└───────────────────────────────┬─────────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────────┐
│                       GAME FRAMEWORK                                    │
│      Core_Classes  │  Core_System  │  Core_GameFeatures                 │
└───────────────────────────────┬─────────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────────┐
│                      GAMEPLAY SYSTEMS                                   │
│  Core_AbilitySystem  │  Core_Interact  │  Core_Physics  │  Core_Effects │
└───────────────────────────────┬─────────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────────┐
│                     DATA & SETTINGS                                     │
│          Core_Data  │  Core_Setup  │  Core_Performance                  │
└───────────────────────────────┬─────────────────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────────────────┐
│                        FOUNDATION                                       │
│         Core_Structure  │  Core_Interfaces  │  Core_Logging             │
└─────────────────────────────────────────────────────────────────────────┘
                     │              │
             Core_Demo       Core_Extras   (auxiliary actors)
```

---

## Plugin Dependencies

| Plugin | Purpose |
|--------|---------|
| Elemental_Collection | Base classes, interfaces, player/team infrastructure |
| AssetLoader | Async primary/secondary asset discovery and loading |
| LoggingMacros | Structured logging macros |
| GameSettings | Settings registry and discrete/scalar value framework |
| AudioModulation | Audio bus/mix control for volume settings |
| ModularGameplay | Modular actor/component extension framework |
| GameplayMessageRouter | Verb message broadcasting |
| Niagara | Particle effects (Core_Extras) |
| ChaosVehiclesPlugin | Wheeled vehicle pawns |

---

## Foundation Modules

### Core_Structure

Provides the shared data types, enums, structs, and gameplay tag definitions used across all other modules.

**Key enums:**
- `EDiplomaticStatus` — None / Unidentified / Self / Ally / Neutral / Enemy / Selected
- `EMeasurementSystem` — Metric / Imperial
- `EVehicleLightType` — Front / Rear / BlinkFront / BlinkRear

**Key structs:**
- `FFaction` — Name, UIColor, DiplomaticStatus
- `FFlightModel` / `FFlightModelType` — Helicopter flight physics configuration (hovering force, cyclic gains, engine thrust, damping)
- `FHelicopterSetupData` — Complete helicopter control parameters (pitch/roll/yaw speeds, momentum, collective limits, SAS torque)
- `FVehicleLight` / `FVehicleLightSetup` — Light type, transform, color, IES profile; pre-populated with 8 default lights
- `FCollisionSetupData` — Collision response/channel configuration
- `FGuidManagerEntry` — Object path for GUID tracking (DataTable row)

**Gameplay tags** (namespace `GameplayTags_Core`):
- Initialization states: `InitState_Spawned` → `DataAvailable` → `DataInitialized` → `GameplayReady`
- Ability failure: `Ability_ActivateFail_IsDead/Cooldown/Cost/TagsBlocked/TagsMissing/Networking/ActivationGroup`
- Status: `Status_Crouching`, `Status_AutoRunning`, `Status_Death`, `Status_Death_Dying`, `Status_Death_Dead`
- Movement modes: `Movement_Mode_Walking/NavWalking/Falling/Swimming/Flying/Custom`
- Input tags: `InputTag_Move`, `InputTag_Look_Mouse/Stick`, `InputTag_Crouch`, `InputTag_AutoRun`
- Gameplay events: `GameplayEvent_Death`, `GameplayEvent_Reset/RequestReset`
- SetByCaller: `SetByCaller_Damage`, `SetByCaller_Heal`
- Cheats: `Cheat_GodMode`, `Cheat_UnlimitedHealth`

---

### Core_Interfaces

Decoupling interfaces that break circular dependencies between higher-level modules.

**Key interfaces:**
- `IAbilitySystemComponentClassProviderInterface` / `CoreAbilitySystemComponentClassRegistry` — Register and retrieve the ASC class without depending on `Core_AbilitySystem`
- `IPawnDataProviderInterface` — Query pawn data by controller, default, or path
- `IPawnDataPawnClassProviderInterface` — Provide pawn class from data asset
- `IPerformanceStatProviderInterface` — Query server FPS from game state
- `IGameMode_CoreInterface` — Spawn actors at location from game mode
- `IInteractableTarget` — Gather interaction options and customize event data
- `IKeyboardInputSettingInterface` — Key text lookup, rebinding, customization state
- `ICoreSettingsLocalPlayerInterface` — Access local and shared settings from local player
- `PawnDataAbilitySetApplier` — Static function registry for applying ability sets to ASC

**Key subsystems:**
- `UPawnDataProviderSubsystem` (WorldSubsystem) — World-level pawn data access; provider registered via `SetProviderForWorld`
- `UPerformanceStatSubsystemBase` (GameInstanceSubsystem, abstract) — Stat cache base; concrete implementation in `Core_Performance`

**Performance stat types** (defined here, re-exported by `Core_Performance`):
- `ECoreStatDisplayMode` — Hidden / TextOnly / GraphOnly / TextAndGraph
- `ECoreDisplayablePerformanceStat` — ClientFPS, ServerFPS, IdleTime, FrameTime variants, Ping, PacketLoss, PacketRate, PacketSize, Latency variants (18 stats total)
- `FCorePerformanceStatGroup` — Tag-query-filtered group of allowed stats

---

### Core_Logging

Declares the `Core_Logging` log category used internally during module startup/shutdown.

---

## Data & Settings Modules

### Core_Data

Stores settings persistence classes, experience definitions, and game-specific data assets.

**Settings:**
- `UCoreSettingsLocal` (extends `UGameUserSettings`) — Machine-local settings:
  - Display gamma (2.2 default), frame rate limits (OnBattery, InMenu, WhenBackgrounded)
  - Audio volumes (Overall, Music, SoundFX, Dialogue, VoiceChat) via `USoundControlBus` map
  - Safe zone scale, controller platform/preset, input config name
  - Performance stat display state map (`ECoreDisplayablePerformanceStat → ECoreStatDisplayMode`)
  - Audio output device ID, replay count to keep (default 10), mobile FPS limit (default 60)
- `UCoreSettingsShared` (extends `ULocalPlayerSaveGame`) — Cloud-safe per-player settings:
  - Color blind mode (Off/Deuteranope/Protanope/Tritanope) and strength
  - Force feedback toggle; gamepad deadzone (move/look); trigger haptics (strength, threshold, position)
  - Subtitles (enabled, text size/color/border, background opacity)
  - Background audio policy; pending culture / language changes
  - Mouse sensitivity (X/Y, targeting multiplier, invert axes)
  - Gamepad sensitivity presets (`ECoreGamepadSensitivity`: Slow → Insane)
  - `ChangeValueAndDirty<T>()` template for dirty-flag pattern
- `ECoreColorBlindMode`, `ECoreAllowBackgroundAudioSetting`, `ECoreGamepadSensitivity`

**Experience system:**
- `UExperienceDefinition` (PrimaryDataAsset) — List of game features to enable, default pawn data, actions, action sets
- `UExperienceActionSet` (PrimaryDataAsset) — Reusable bundle of game feature actions

**Data assets:**
- `UActorCollectionDataAsset` — Default / Dummy / Destroyed actor class pointers (PreLoadingDataAsset)
- `UFactionInformationDataAsset` — Faction list with diplomatic status and colors
- `UHelicopterSetupDataAsset` — Helicopter flight config
- Additional: `EngineSoundDataAsset`, `GuidDataAsset`, `ReplacementActorDataAsset`, `StaticMeshAnimationDataAsset`, `StaticTurretMeshSetupDataAsset`, `VehicleLightsSetupDataAsset`, `CollisionControlDataAsset`

---

### Core_Setup

Implements the settings UI registry using Unreal's `GameSettings` framework.

**`UCoreGameSettingRegistry`** — Creates and owns five `UGameSettingCollection` instances:
- **Video** — Resolution, window mode, overall quality (Low/Medium/High/Epic/Custom), per-setting scalability sliders, frame rate limits, safe zone editor, auto-benchmark
- **Audio** — Per-bus volumes, audio output device, background audio policy
- **Gameplay** — Language selection, subtitles (enabled, size, color, border, opacity), color blind mode/strength, force feedback
- **Mouse & Keyboard** — Sensitivity (X/Y, targeting), invert axes; keyboard rebinding via `UCoreSettingKeyboardInput`
- **Gamepad** — Deadzone, trigger haptics, sensitivity presets, mobile FPS type

**Custom setting classes:**
- `UCoreSettingKeyboardInput` (implements `IKeyboardInputSettingInterface`) — EnhancedInput profile-based keybinding, slot-based get/set/reset
- `UCoreSettingValueDiscrete_Language` — Culture-name-indexed language options
- `UCoreSettingValueDiscrete_OverallQuality` — Low/Med/High/Epic/Custom scalability enum
- `UCoreSettingValueDiscrete_Resolution` — Full/windowed/borderless resolution lists filtered per window mode
- `UCoreSettingValueDiscrete_PerfStat` — Per-stat display mode selector
- `UCoreSettingValueDiscrete_MobileFPSType`, `UCoreSettingValueDiscrete_PlanesInTheSky`
- `UCoreSettingValueDiscreteDynamic_AudioOutputDevice`
- `UCoreSettingValueScalarDynamic_SafeZoneValue` + `UCoreSettingAction_SafeZoneEditor`

Module startup registers `UCoreSettingsLocal::OnExperienceLoaded` and exposes settings under "War Collection" category.

---

### Core_Performance

Frame-by-frame performance metrics collection and UI-facing statistics system.

**`UCorePerformanceStatSubsystem`** (GameInstanceSubsystem) — Registers `FCorePerformanceStatCache` with GEngine's performance data consumer system on init.

**`FCorePerformanceStatCache`** (implements `IPerformanceDataConsumer`) — Per-frame collector:
- Reads `FFrameData` from the engine (game/render/RHI/GPU thread times)
- Queries `ILatencyMarkerModule` for total/game/render latency
- Queries game state for server FPS; queries player state for ping, packet loss/rate/size
- Stores results in `FSampledStatCache` circular buffers (125 samples each)

**`FSampledStatCache`** — Circular buffer with `GetAverage()`, `GetMin()`, `GetMax()`, `ForEachCurrentSample()`

**`UCorePerformanceSettings`** (UDeveloperSettingsBackedByCVars, implements `IPerformanceStatSettingsProvider`) — Desktop frame rate limits `{30,60,120,144,160,165,180,200,240,360}` Hz; per-stat visibility groups

**`UCorePlatformSpecificRenderingSettings`** (UPlatformSettings, implements `IPlatformRenderingSettingsProvider`) — Device profile suffix, quality variants, frame pacing mode (Desktop/Console/Mobile), mobile FPS limits `{20,30,45,60,90,120}` Hz

CSV profiling category `CorePerformance` (disabled by default); records latency stats when `ILatencyMarkerModule` is present.

---

## Game Framework Modules

### Core_Classes

The largest runtime module — all core actor, component, and UI classes.

**Characters:**
- `ACoreCharacter` (extends `ACombinedBundleCharacter`) — Implements 8 interfaces: `IAbilitySystemInterface`, `IGameplayTagAssetInterface`, `ICharacterGroundInfoProviderInterface`, `IDeathEventInterface`, `IHumanoidReplicationInterface`, `IMovementModeTagInterface`, `ITeamAgentInterface`, `IReplicationGraphCharacterInterface`
  - `FCoreReplicatedAcceleration` — Compressed acceleration (XYRadians uint8, XYMagnitude uint8, Z int8)
  - `FCoreSharedRepMovement` — NetSerialized shared movement (with `TStructOpsTypeTraits` for shared serialization)
  - `FastSharedReplication()` NetMulticast, unreliable — bandwidth-optimized movement broadcast
  - Movement mode tag tracking, crouching tag count, death/respawn hooks

**Game mode / game state:**
- `AGameMode_Core` (extends `AGameModeBase`) — Experience-driven: waits for `OnExperienceLoaded`, chooses pawn class via `GetPawnDataForController`, handles seamless restart flow
  - Delegate: `FOnCoreGameModePlayerInitialized OnGameModePlayerInitialized`
- `ACoreGameState` (extends `AModularGameStateBase`) — Hosts the shared `UAbilitySystemComponent`; replicates `ServerFPS` and `RecorderPlayerState`; multicasts `FVerbMessage` to all clients

**Player:**
- `UCoreLocalPlayer` (extends `UCommonLocalPlayer`) — Owns `UCoreSettingsLocal` and `UCoreSettingsShared`; implements team interface; lazy-loads shared settings from disk

**HUD / UI:**
- `ACoreHUD` — Base HUD
- `UCoreActivatableWidget` — `ECoreWidgetInputMode` (Default/GameAndMenu/Game/Menu), mouse capture mode
- `UCoreWidgetFactory` — Data-driven widget class lookup (BlueprintNativeEvent)
- `UCoreUIManagerSubsystem` — Syncs HUD visibility with root layout
- `UCoreLoadingScreenSubsystem` — Manages loading screen widget class with `OnLoadingScreenWidgetChanged` delegate
- `UCoreTabListWidgetBase` — Pre-registered tab descriptors (`FCoreTabDescriptor`) with hidden state, dynamic tab support
- `UCorePerfStatWidgetBase` — Displays a single `ECoreDisplayablePerformanceStat` with optional `UCorePerfStatGraph` (wraps `SLatencyGraph` Slate widget)

**Indicator system:**
- `UCoreIndicatorManagerComponent` (ControllerComponent) — Manages `UCoreIndicatorDescriptor` list; `OnIndicatorAdded/Removed` events
- `UCoreIndicatorDescriptor` — Full descriptor: component/socket, projection mode (`EActorCanvasProjectionMode`), clamping, screen/world offsets, priority

**Actors / Pawns:**
- `ACombinedBundleActor` — Base actor with async asset loading, game feature listener, interact delegate, entity information interface
- `CombinedBundlePawn`, `CombinedWheeledVehiclePawn` — Base pawn types
- `DefinedCollectionSpawningActor` — Spawns actor from `UActorCollectionDataAsset` using `ECollectionSpawnType`
- `DummyActor`, `CombinedDestroyedActor` — Default/destroyed state stand-ins

**Components:**
- `UPawnDrivingActorComponent_Base` — Base for components that need pawn owner + controller access; `ProcessPossess()` hook

**Subsystems:**
- `UGuidManageSubsystem` — Object path → GUID lookup from data table

---

### Core_System

Engine and infrastructure classes for asset management, replication, and server/client plumbing.

**Asset management:**
- `UAssetManager_Core` (extends `UAssetManager`) — Singleton with startup job queue (`FAssetManagerStartupJob_Core`); thread-safe `AddLoadedAsset()`; loads `UGameData_Core` and default `UPawnData_Core` via config
- `UGameData_Core` — Damage/Heal/DynamicTag gameplay effect soft class references
- `UPawnData_Core` — Pawn class, ability sets to grant, tag relationship mapping (implements `IPawnDataAbilitySetProviderInterface`, `IPawnDataTagRelationshipProviderInterface`, `IPawnDataPawnClassProviderInterface`)
- `FCoreBundles::Equipped` — Asset bundle name constant

**Game instance / engine / session:**
- `UGameInstance_Core` (extends `UCommonGameInstance`) — Handles experience-loaded callback, replay support, network encryption tokens; debug encryption key (not for production)
- `UGameEngine_Core` — Custom game engine with one-time init hook
- `AGameSession_Core` — Disables auto-login; hooks match start/end
- `ACoreWorldSettings` — Per-map `DefaultGameplayExperience` soft class reference; `ForceStandaloneNetMode` editor flag

**Replication graph:**
- `ECoreClassRepNodeMapping` — NotRouted / RelevantAllConnections / Spatialize_Static / Spatialize_Dynamic / Spatialize_Dormancy
- `UCoreReplicationGraphSettings` (DeveloperSettings, CVars prefixed `Core.RepGraph.*`) — Grid cell size (10000 UU), spatial bias (-200000), fast shared path toggle, dynamic frequency buckets
- `UCoreReplicationGraph` — Spatial grid + always-relevant node; configurable per-class routing; streaming level actor lists
- `UCoreReplicationGraphNode_AlwaysRelevant_ForConnection` — Per-connection always-relevant; handles streaming level visibility
- `UCoreReplicationGraphNode_PlayerStateFrequencyLimiter` — Limits player state replication to `TargetActorsPerFrame = 2`

**Significance management:**
- `USignificanceManager_Core` — Minimal extension of `USignificanceManager`

**Utilities:**
- `UActorUtilities` — `SwitchOnNetMode()` Blueprint function (Standalone/DedicatedServer/ListenServer/Client enum)
- `UDevelopmentStatics_Core` — `ShouldSkipDirectlyToGameplay()`, `ShouldLoadCosmeticBackgrounds()`, `CanPlayerBotsAttack()`, `FindClassByShortName<T>()`
- `UCorePawnDataProviderSubsystem` — Concrete world subsystem that queries `UAssetManager_Core` for pawn data

---

### Core_GameFeatures

GameFeature plugin actions and project policy for modular gameplay extension.

**Project policy:**
- `UCoreGameFeaturePolicy` (extends `UDefaultGameFeaturesProjectPolicies`) — Manages observer list; controls client/server data loading; filters allowed plugins; registers `UCoreGameFeature_HotfixManager` and `UCoreGameFeature_AddGameplayCuePaths` as observers

**Base action:**
- `UGameFeatureAction_WorldActionBase` — Abstract; calls `AddToWorld()` for each game instance start via `GameInstanceStartHandles`

**Actions:**
| Action | What it does |
|--------|-------------|
| `UGameFeatureAction_AddAbilities` | Grants `UGameplayAbility` and `UAttributeSet` classes to actors via `FGameFeatureAbilitiesEntry`; tracks handles per context |
| `UGameFeatureAction_AddInputBinding` | Adds input bindings to pawns via pawn extension events |
| `UGameFeatureAction_AddInputContextMapping` | Registers `UInputMappingContext` to `UEnhancedInputLocalPlayerSubsystem`; priority-based; registers with settings if `bRegisterWithSettings=true` |
| `UGameFeatureAction_AddWidgets` | Adds HUD layout layers (`FHUDLayoutRequest`) and UI slots (`FHUDElementEntry`) to actor via component requests |
| `UGameFeatureAction_AddGameplayCuePath` | Registers `FDirectoryPath` list with GameplayCue manager |
| `UGameFeatureAction_SplitscreenConfig` | Votes to disable splitscreen using a static global vote counter |

---

## Gameplay Systems Modules

### Core_AbilitySystem

Full GAS implementation adapted from Lyra.

**Abilities:**
- `UCoreGameplayAbility` — Base ability with activation policy (`OnInputTriggered/WhileInputActive/OnSpawn`), activation group (`Independent/Exclusive_Replaceable/Exclusive_Blocking`), pluggable `UCoreAbilityCost` list, failure tag → message/montage mapping, camera mode management
- `UCoreGameplayAbility_Death` — `StartDeath()` / `FinishDeath()` BP-callable; optional `bAutoStartDeath`
- `UCoreGameplayAbility_Jump` — `CharacterJumpStart()` / `CharacterJumpStop()`
- `UCoreGameplayAbility_Reset` — Triggers player reset; sends `FCorePlayerResetMessage`
- `UCoreGamePhaseAbility` — Carries `FGameplayTag GamePhaseTag`; signals `UCoreGamePhaseSubsystem` on activate/end

**Ability system component:**
- `UCoreAbilitySystemComponent` — Custom ASC implementing `IAbilitySystemInputInterface` and `IAbilitySystemTagRelationshipReceiverInterface`
  - Per-frame input processing: `InputPressedSpecHandles / InputReleasedSpecHandles / InputHeldSpecHandles`
  - Activation group enforcement via `ActivationGroupCounts[]`
  - `AddDynamicTagGameplayEffect()` / `RemoveDynamicTagGameplayEffect()` for tag-as-effect
  - `CancelAbilitiesByFunc(TShouldCancelAbilityFunc)` for filtered cancellation
  - Tag relationship mapping integration

**Attribute sets:**
- `UCoreHealthSet` — Health / MaxHealth / Healing / Damage (meta); `OnHealthChanged`, `OnMaxHealthChanged`, `OnOutOfHealth` delegates; immunity tag support
- `UCoreCombatSet` — BaseDamage / BaseHeal
- `ATTRIBUTE_ACCESSORS` macro generates typed getter/setter/initializer for all attributes

**Ability costs:**
- `UCoreAbilityCost` (abstract) — `CheckCost()` / `ApplyCost()` / `bOnlyApplyCostOnHit`
- `UCoreAbilityCost_PlayerTagStack` — Consume tag stack from player state
- `UCoreAbilityCost_ItemTagStack` — Consume tag from item instance; failure response tag
- `UCoreAbilityCost_InventoryItem` — Consume quantity of item type from inventory manager

**Ability sets:**
- `UCoreAbilitySet` (PrimaryDataAsset) — Bundles abilities + effects + attribute sets; `GiveToAbilitySystem()` returns `FCoreAbilitySet_GrantedHandles` for later removal

**Tag relationships:**
- `UAbilityTagRelationshipMapping` (DataAsset) — `FAbilityTagRelationship` list: per-ability-tag block/cancel/require/block-activation rules

**Effect context:**
- `FCoreGameplayEffectContext` — Extends `FGameplayEffectContext` with `CartridgeID` and `AbilitySourceObject`
- `UCoreAbilitySystemGlobals` — Allocates `FCoreGameplayEffectContext`

**Executions:**
- `UCoreDamageExecution` — SetByCaller magnitude → Health reduction via `Healing/Damage` meta attributes
- `UCoreHealExecution` — Symmetric heal path

**Phases:**
- `UCoreGamePhaseSubsystem` (WorldSubsystem) — Hierarchical game phases driven by gameplay tags; siblings cancelled on new phase start; `StartPhase()`, `WhenPhaseStartsOrIsActive()`, `WhenPhaseEnds()` with exact/partial tag matching

**Global ability subsystem:**
- `UGlobalAbilitySubsystem` (WorldSubsystem) — Apply/remove abilities and effects to all registered ASCs at once

**Gameplay Cue Manager:**
- `UGameplayCueManager_Core` — Preloads tag-referenced cues; handles post-GC cleanup; async library loading; `DumpGameplayCues()` console command

**Failure messages:**
- `FAbilitySimpleFailureMessage` — PlayerController + failure tags + user-facing text (via gameplay message)
- `FAbilityMontageFailureMessage` — Adds avatar actor and failure montage

---

### Core_Interact

Interaction option system (adapted from Lyra).

- `IInteractableTarget` (from `Core_Interfaces`) — `GatherInteractionOptions(FInteractionQuery, FInteractionOptionBuilder)` / `CustomizeInteractionEventData()`
- `FInteractionQuery` — Requesting avatar (weak), requesting controller (weak), optional object data
- `FInteractionOption` — Text/SubText, ability-to-grant, target ASC + ability handle, widget class
- `FInteractionOptionBuilder` — Scoped builder that auto-sets `InteractableTarget` on every added option

---

### Core_Physics

Physics infrastructure and collision channel registration.

**Collision channels** (defined as macros in `CoreCollisionChannels.h`):
- `Core_TraceChannel_Interaction` = `ECC_GameTraceChannel1` — Interact traces
- `Core_TraceChannel_Weapon` = `ECC_GameTraceChannel2` — Weapon traces (physics assets)
- `Core_TraceChannel_Weapon_Capsule` = `ECC_GameTraceChannel3` — Weapon traces (capsules)
- `Core_TraceChannel_Weapon_Multi` = `ECC_GameTraceChannel4` — Multi-pawn weapon traces
- Channel 5 reserved for aim assist by ShooterCore game feature

**`UCorePhysicalMaterialWithTags`** (extends `UPhysicalMaterial`, implements `IPhysicalMaterialWithTagsInterface`) — Adds `FGameplayTagContainer Tags` for surface-type reasoning in ability calculations without creating a hard dependency on `Core_Physics` from `Core_AbilitySystem`.

Module startup registers `ECC_GameTraceChannel1` with `InteractionCollisionChannelRegistry`; shutdown resets to `ECC_WorldDynamic`.

---

### Core_Effects

Placeholder module with `GameplayAbilities` and `GameplayTags` public dependencies. No classes defined yet; serves as the module registration point for future gameplay effect infrastructure.

---

## Infrastructure Modules

### Core_HotfixSystem

Runtime patching via Epic's online hotfix manager.

- `UCoreHotfixManager` (extends `UOnlineHotfixManager`) — Intercepts game.ini and device-profile hotfixes; deferred asset patching via `FTSTicker`; `OnPendingGameHotfixChanged` multicast delegate for UI
- `UCoreRuntimeOptions` (extends `URuntimeOptionsBase`) — Console variable-backed runtime toggles prefixed `ro.`
- `UCoreTextHotfixConfig` (extends `UDeveloperSettings`) — `TArray<FPolyglotTextData> TextReplacements`; applies via `FTextLocalizationManager` on init and config reload

---

### Core_Replay

Client replay recording, playback, and management.

- `UWarCollectionReplaySubsystem` (GameInstanceSubsystem) — `RecordClientReplay()`, `PlayReplay()`, `SeekInActiveReplay()`, `CleanupLocalReplays(NumToKeep)`; platform support via `TAG_Platform_Trait_ReplaySupport`
- `UWarCollectionReplayListEntry` / `UWarCollectionReplayList` — Wraps `FNetworkReplayStreamInfo`; exposes friendly name, timestamp, duration, viewer count, live state to Blueprints
- `UAsyncAction_QueryReplays` (BlueprintAsyncActionBase) — Non-blocking stream enumeration; fires `QueryComplete(UWarCollectionReplayList*)` delegate

Cleanup iterates streams, deletes oldest until count ≤ `NumberOfReplaysToKeep` (from `UCoreSettingsLocal`).

---

### Core_Tests

Test automation and external game control.

- `UCoreGameplayRpcRegistrationComponent` (extends `UExternalRpcRegistrationComponent`) — HTTP endpoint registration grouped by game state:
  - **Always-on**: `POST /core/cheatcommand` — routes JSON `{command}` to `PlayerController::ConsoleCommand`
  - **Frontend**: `GET /matchmaking/status`, `POST /matchmaking/find`
  - **In-match**: `POST /player/status` — fires weapon ability; `GET /player/status` — returns health + inventory JSON via `IHealthComponentInterface` / `IInventoryManagerInterface`
- `UCoreTestControllerBootTest` — Gauntlet boot test; `IsBootProcessComplete()` returns true after 20 s if World + GameInstance are valid

---

### Core_Development

Developer ergonomics for editor and PIE workflows.

**`UCoreDeveloperSettings`** (DeveloperSettings + CVars):
- `ExperienceOverride` — Skip to specific experience in PIE
- `bOverrideBotCount` / `OverrideNumPlayerBotsToSpawn` — Control AI population
- `bTestFullGameFlowInPIE` — Force full match flow instead of shortcutting
- `CheatsToRun` — Auto-execute cheats on `CheatManagerCreated` or `PlayerPawnPossession`
- `LogGameplayMessages` CVar — Toggle `GameplayMessageSubsystem` logging

**`UCorePlatformEmulationSettings`** (DeveloperSettings + CVars):
- `PretendPlatform` / `PretendBaseDeviceProfile` — Emulate another platform's device profile in PIE
- `AdditionalPlatformTraitsToEnable/Suppress` — Modify `CommonUI` platform traits
- `bApplyFrameRateSettings/FrontEndPerformance/DeviceProfilesInPIE` — Apply production settings during editing

**`UCoreBotCheats`** (CheatManagerExtension):
- `AddPlayerBot()` / `RemovePlayerBot()` — Exec commands that call `IBotSpawnProviderInterface` via reflection

---

## Auxiliary Modules

### Core_Demo

Demo and showcase support.

- `EDemoDisplayType` — Round / SquareL / RoomL / DescriptionOnly / SquareLFlatWall
- `ADemoDisplayActor` — Text render + dynamic material color; `FOnDemoColorChanged` delegate
- `ADemoDefinedCollectionActor` (extends `ADefinedCollectionSpawningActor`) — Spawns entity collections for demos

---

### Core_Extras

Miscellaneous gameplay actors.

- `AGameplayPadActor` (extends `ACombinedBundleActor`) — Box volume, text render, pad + base static meshes
- `AGiftPlacingActor` — Gift mesh with 6 decoy positions, Niagara decoy effect, sound cues, UMG solve widget; activated on `ActorBeginOverlap`

---

## Editor Module

### Core_Collection_Editor

Editor tooling, asset validation, and PIE customization.

**Engine / PIE:**
- `UCoreEditorEngine` (extends `UUnrealEdEngine`) — First-tick setup; PIE instance pre-creation hook
- `UCoreDeveloperSettings::OnPlayInEditorStarted()` / `UCorePlatformEmulationSettings::OnPlayInEditorStarted()` — Apply PIE overrides

**Editor mode** (`EM_Core_CollectionEditorMode`):
- `UCore_CollectionSimpleTool` (USingleClickTool) — Click to show actor info via raycast
- `UCore_CollectionInteractiveTool` (UInteractiveTool + IClickDragBehaviorTarget) — Click-drag distance measurement; Shift = move second point

**Asset validation:**
- `UEditorValidator` (abstract) — Base with static `ValidateCheckedOutContent()`, `ValidatePackages()`, `ValidateProjectSettings()`, Perforce integration
- `UEditorValidator_Blueprints` — Compile validation for Blueprint assets
- `UEditorValidator_Load` — Load-time warning/error capture via `FCoreValidationMessageGatherer`
- `UEditorValidator_MaterialFunctions` — Material function validation
- `UEditorValidator_SourceControl` — Source control state validation
- `FCoreValidationMessageGatherer` — `FOutputDevice` that captures log warnings/errors with pattern filtering
- `UContentValidationCommandlet` — CLI content validation with Perforce file enumeration and MCP template export

**Asset factories:**
- `FAssetTypeActions_CoreContextEffectsLibrary` — Content browser integration (green color, Gameplay category)
- `UCoreContextEffectsLibraryFactory` — Creates ContextEffectsLibrary assets

**Property customization:**
- `FEntityDefinitionCustomization` — Dropdown picker for `FEntityDefinition.SelectedEntity` property

**GameplayCue editor integration:**
- Provides default cue classes (Burst, BurstLatent, Looping), interface class list, and asset path for the GameplayAbilities editor

---

## Key Design Patterns

| Pattern | Where used |
|---------|------------|
| **Static registry** | `CoreAbilitySystemComponentClassRegistry`, `PawnDataAbilitySetApplier`, `InteractionCollisionChannelRegistry` |
| **Dual-layer settings** | `UCoreSettingsLocal` (machine) + `UCoreSettingsShared` (cloud/player) |
| **Provider registration** | Performance stats, platform rendering settings, pawn data — registered via interface namespaces on module startup |
| **World subsystem + delegate** | `UCoreGamePhaseSubsystem`, `UGlobalAbilitySubsystem`, `UCorePawnDataProviderSubsystem` |
| **Compressed replication structs** | `FCoreReplicatedAcceleration` (3 bytes), `FCoreSharedRepMovement` with shared serialization |
| **Spatial replication graph** | `UCoreReplicationGraph` — grid spatialization, dormancy, frequency limiting, fast shared path |
| **Experience-driven initialization** | GameMode waits for `UExperienceDefinition` loaded, then calls `GenericPlayerInitialization` and chooses pawn |
| **Hierarchical game phases** | Phase abilities cancel sibling phases with overlapping tag hierarchy |
| **GameFeature actions** | Per-context data maps; component request handles; delegate-based activation/deactivation |
| **Async action pattern** | `UAsyncAction_QueryReplays` for non-blocking replay enumeration |
| **HTTP RPC registry** | State-gated endpoint registration in `Core_Tests` for Gauntlet/automation |
