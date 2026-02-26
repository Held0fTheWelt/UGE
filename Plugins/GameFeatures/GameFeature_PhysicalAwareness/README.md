# GameFeature_PhysicalAwareness

Unreal Engine **5.6** — Game Feature Plugin in the UGE Framework.

Provides a collision-based touch-sense system that lets a pawn detect, track, and query physically overlapping actors.

---

## Plugin Configuration

| Property | Value |
|----------|-------|
| Module | `GameFeature_PhysicalAwarenessRuntime` (Runtime, Default) |
| Initial State | **Active** (`BuiltInInitialFeatureState: Active`) |
| Explicitly loaded | `true` |
| Sealed | `true` |

**Plugin dependencies:** `AssetLoader` · `Elemental_Collection` · `Core_Collection`

---

## Position in the UGE Layer Model

```
┌─────────────────────────────────────────────────────────────┐
│  Game Features      GameFeature_PhysicalAwareness  ← here   │
├─────────────────────────────────────────────────────────────┤
│  Equipment Plugins  Humanoids · Helicopters · …             │
├─────────────────────────────────────────────────────────────┤
│  Core_Collection    PawnDrivingActorComponent_Base           │
│                     ISceneComponentCatchInterface            │
├─────────────────────────────────────────────────────────────┤
│  Elemental_Collection  IPhysicalSenseInterface               │
│                        IInteractControlInterface             │
│                        INoTouchCollisionInterface            │
├─────────────────────────────────────────────────────────────┤
│  AssetLoader        UPreLoadingDataAsset                     │
└─────────────────────────────────────────────────────────────┘
```

---

## Directory Structure

```
GameFeature_PhysicalAwareness/
├── GameFeature_PhysicalAwareness.uplugin
├── Content/
│   └── GameFeature_PhysicalAwareness.uasset
└── Source/GameFeature_PhysicalAwarenessRuntime/
    ├── GameFeature_PhysicalAwarenessRuntime.Build.cs
    ├── Public/
    │   ├── Components/
    │   │   └── PhysicalAwarenessSenseComponent.h
    │   ├── DataAssets/
    │   │   └── PhysicalAwarenessSenseDataAsset.h
    │   ├── Logging/
    │   │   └── Logging_GF_PhysicalAwarenessSense.h
    │   └── GameFeature_PhysicalAwarenessRuntimeModule.h
    └── Private/
        ├── Components/
        │   ├── PhysicalAwarenessSenseComponent.cpp        — Constructor, BeginPlay, interface implementations
        │   ├── PhysicalAwarenessSenseComponent_Sense.cpp  — Overlap events, actor filtering
        │   └── PhysicalAwarenessSenseComponent_Setup.cpp  — Asset loading, collision component
        ├── DataAssets/
        │   └── PhysicalAwarenessSenseDataAsset.cpp
        ├── Logging/
        │   └── Logging_GF_PhysicalAwarenessSense.cpp
        └── GameFeature_PhysicalAwarenessRuntimeModule.cpp
```

---

## Core Classes

### `UPhysicalAwarenessSenseComponent`

**Inheritance:**
```
UActorComponent
  └─ UCombinedBundleActorComponent   — Asset loading framework, GameFeatureName tracking
       └─ UPawnDrivingActorComponent_Base  — PawnOwner / PawnController, possession events
            └─ UPhysicalAwarenessSenseComponent
                 implements IPhysicalSenseInterface
```

**Constructor Defaults:**

| Property | Value | Purpose |
|----------|-------|---------|
| `GameFeatureName` | `"PhysicalAwareness"` | Links asset loading to this feature |
| `bDoNotRegisterOnPossessControl` | `true` | Does not automatically register with the controller |
| `bComponentHasComponentAssetToLoad` | `false` | Uses the owning actor's asset, not a component-level asset |

**Transient Runtime Properties:**

| Property | Type | Purpose |
|----------|------|---------|
| `bDoesTrackActorsForGameManagement` | `bool` | Master switch — disables all collision tracking when `false` |
| `bCanSenseActors` | `bool` | Controls whether sense queries return results |
| `bCanTouchActors` | `bool` | Controls whether touch interactions are permitted |
| `TouchableActors` | `TArray<TSoftObjectPtr<AActor>>` | All currently overlapping, registered actors |
| `CurrentTouchedActor` | `AActor*` | Explicitly set "focused" actor (externally managed) |

---

### `UPhysicalAwarenessSenseDataAsset`

Configuration data asset. Loaded via `AssetLoader` and consumed by `SetupLoadedAsset()`.

**Inheritance:** `UPreLoadingDataAsset` (Primary Asset Type: `"PhysicalAwareness"`)

**Editable Properties** (category `"Physical Awareness"`):

| Property | Default | Purpose |
|----------|---------|---------|
| `bDoesTrackActorsForGameManagement` | `false` | Enables collision tracking; when `false`, the collision component is deactivated and `TouchableActors` is cleared |
| `bCanSenseActors` | `false` | Enables general sensing |
| `bCanTouchActors` | `false` | Enables touch interactions |

---

## Flow

### Initialization

```
BeginPlay()
  ├─ IsGameFeatureIgnored()?  →  return (no-op)
  ├─ TouchableActors.Empty()
  └─ SetCollisionComponentActive(true)
       └─ GetCollisionComponent()
            └─ ISceneComponentCatchInterface::GetSceneComponent(Owner, "Collision")
                 └─ Cast<UPrimitiveComponent>
       └─ BindDynamic OnComponentBeginOverlap → OnBeginOverlap
       └─ BindDynamic OnComponentEndOverlap  → OnEndOverlap

[Asset loaded by UCombinedBundleActorComponent]
SetupLoadedAsset()
  ├─ Checks PrimaryAssetType == "PhysicalAwareness"
  ├─ Cast → UPhysicalAwarenessSenseDataAsset
  ├─ Copies bDoesTrackActorsForGameManagement / bCanSenseActors / bCanTouchActors
  └─ !bDoesTrackActorsForGameManagement?
       └─ SetCollisionComponentActive(false) + TouchableActors.Empty()
```

### Runtime — Overlap Filtering

```
OnBeginOverlap(OtherActor, OtherComp)
  ├─ !bDoesTrackActorsForGameManagement  →  return
  ├─ OtherComp implements INoTouchCollisionInterface  →  return
  └─ SenseTouchActor(OtherActor, true)
       ├─ Actor has no component with IInteractControlInterface  →  return
       └─ RegisterObject(Actor, ST_TOUCH)
            └─ TouchableActors.AddUnique(Actor)

OnEndOverlap(OtherActor, OtherComp)
  ├─ !bDoesTrackActorsForGameManagement  →  return
  ├─ OtherComp implements INoTouchCollisionInterface  →  return
  └─ SenseTouchActor(OtherActor, false)
       └─ ForgetObject(Actor, ST_TOUCH)
            └─ TouchableActors.Remove(Actor)
```

**Filter rules summary:**

| Condition | Result |
|-----------|--------|
| `bDoesTrackActorsForGameManagement == false` | Overlap completely ignored |
| `OtherComp` implements `INoTouchCollisionInterface` | Overlap ignored |
| `OtherActor` has no component with `IInteractControlInterface` | Actor not registered |
| All conditions pass | Actor added to / removed from `TouchableActors` |

---

## Interface Contract (`IPhysicalSenseInterface`)

All methods are implemented `private` and callable only through the interface.

| Method | Return | Behavior |
|--------|--------|----------|
| `CanTouchActor(Actor)` | `bool` | `true` if `Actor` is in `TouchableActors` |
| `CanTouchCurrentActor()` | `bool` | `true` if `TouchableActors.Num() > 0` |
| `GetCurrentTouchActor()` | `AActor*` | `TouchableActors[0]` or `nullptr` |
| `IsTouchingCurrentActor()` | `bool` | `true` if `CurrentTouchedActor != nullptr` |
| `SetTouchingCurrentActor(Actor)` | `void` | Sets `CurrentTouchedActor`; `nullptr` is valid |
| `GetTouchableActors()` | `TArray<TSoftObjectPtr<AActor>>` | Returns `TouchableActors` |

> `GetCurrentTouchActor()` and `CanTouchCurrentActor()` are based on the **first element** in `TouchableActors`, not on `CurrentTouchedActor`.
> `CurrentTouchedActor` is a separate, externally-set focus actor.

---

## Requirements for the Owning Actor

The actor to which `UPhysicalAwarenessSenseComponent` is added **must** implement:

| Interface | Purpose |
|-----------|---------|
| `ISceneComponentCatchInterface` | Provides the collision component via the slot name `"Collision"` |

The slot name `"Collision"` must return a `UPrimitiveComponent` — otherwise the sense component remains inactive and logs an error.

---

## CoreInformation Editor Integration

`GetSettingsVariableSets_Implementation()` provides one `FInformationSet` group with the category `"Sense Information"` and three values — `DoesTrackActors`, `CanSenseActors`, `CanTouchActors` — for display in the CoreInformation editor panel.

---

## Logging

| Category | Contents |
|----------|----------|
| `Log_PhysicalSenseComponent` | General lifecycle (BeginPlay, interface calls) |
| `Log_PhysicalSenseComponent_Setup` | Asset loading, collision component binding, `SetupLoadedAsset` |
| `Log_PhysicalSenseComponent_Runtime` | Overlap events, registering / forgetting actors |

All logs use `UE_LOGFMT` with fields `Time`, `Line`, `Class` and context-specific fields (e.g. `Actor`, `Result`).

---

## Module Dependencies

| Module | Visibility | Reason |
|--------|------------|--------|
| `Core` | Public | UE base types |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard UE runtime |
| `AssetLoader` | Private | `UPreLoadingDataAsset`, Asset Manager integration |
| `Elemental_Data` | Private | Data types from Elemental_Collection |
| `Elemental_Interfaces` | Private | `IPhysicalSenseInterface`, `IInteractControlInterface`, `INoTouchCollisionInterface`, `ISceneComponentCatchInterface` |
| `Elemental_Logging` | Private | `GlobalLog`, `StructuredLog` (UE_LOGFMT macros) |
| `Core_Classes` | Private | `UPawnDrivingActorComponent_Base`, `UCombinedBundleActorComponent` |
