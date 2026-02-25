# GameFeature_Interact

Unreal Engine **5.6** — Game Feature Plugin in the UGE Framework.

Provides a collision-based interaction system: actors can define interaction zones that display a UI widget when a pawn approaches, register with the controller, notify sense systems, and respond to Interact input.

---

## Plugin Configuration

| Property | Value |
|----------|-------|
| Modules | `GameFeature_InteractRuntime` (Runtime, Default) · `GameFeature_InteractSetup` (Runtime, Default) |
| Initial State | **Active** (`BuiltInInitialFeatureState: Active`) |
| Explicitly loaded | `true` |
| Sealed | `false` |

**Plugin dependencies:** `AssetLoader` · `Elemental_Collection` · `Core_Collection` · `Warfare_Assets` · `Vehicles_Test_Assets`

---

## Position in the UGE Layer Model

```
┌────────────────────────────────────────────────────────────────┐
│  Game Features      GameFeature_Interact           ← here      │
├────────────────────────────────────────────────────────────────┤
│  Equipment Plugins  Humanoids · Helicopters · …                │
├────────────────────────────────────────────────────────────────┤
│  Controllers        AUnifiedPlayerController                   │
│                     IRegisterComponentInterface                 │
├────────────────────────────────────────────────────────────────┤
│  Core_Collection    PawnDrivingActorComponent_Base              │
│                     InteractManageGameInstanceSubsystem         │
├────────────────────────────────────────────────────────────────┤
│  Elemental_Collection  IInteractControlInterface                │
│                        IInteractInterface                       │
│                        IShapeUpdatedInterface                   │
│                        IHumanoidSenseInterface                  │
│                        ISceneComponentCatchInterface            │
├────────────────────────────────────────────────────────────────┤
│  AssetLoader        UPreLoadingDataAsset                        │
└────────────────────────────────────────────────────────────────┘
```

---

## Directory Structure

```
GameFeature_Interact/
├── GameFeature_Interact.uplugin
├── Content/
│   ├── BP_InteractCollisionComponent.uasset
│   ├── WBP_Interact.uasset
│   └── GameFeature_Interact.uasset
└── Source/
    ├── GameFeature_InteractRuntime/
    │   ├── GameFeature_InteractRuntime.Build.cs
    │   ├── Public/
    │   │   ├── Components/
    │   │   │   └── InteractCollisionComponent.h
    │   │   ├── Interfaces/
    │   │   │   └── InteractWidgetInterface.h
    │   │   ├── Widgets/
    │   │   │   └── InteractUserWidgetBase.h
    │   │   ├── Logging/
    │   │   │   └── GF_Interact_Logging.h
    │   │   └── GameFeature_InteractRuntimeModule.h
    │   └── Private/
    │       ├── Components/
    │       │   ├── InteractCollisionComponent.cpp          — Constructor, BeginPlay, lifecycle, limit
    │       │   ├── InteractCollisionComponent_Setup.cpp    — Asset loading, box creation, ShapeUpdated
    │       │   ├── InteractCollisionComponent_Interact.cpp — Overlap events, sense, controller registration
    │       │   └── InteractCollisionComponent_Widget.cpp   — Show/remove widget
    │       ├── Widgets/
    │       │   └── InteractUserWidgetBase.cpp
    │       ├── Logging/
    │       │   └── GF_Interact_Logging.cpp
    │       └── GameFeature_InteractRuntimeModule.cpp
    └── GameFeature_InteractSetup/
        ├── GameFeature_InteractSetup.build.cs
        ├── Public/
        │   ├── GameFeature_InteractSetup.h
        │   ├── GameFeature_Interact_Settings.h
        │   └── GameFeature_Interact_SettingsCustomization.h
        └── Private/
            ├── GameFeature_InteractSetup.cpp
            ├── GameFeature_Interact_Settings.cpp
            └── GameFeature_Interact_SettingsCustomization.cpp
```

---

## Core Classes

### `UInteractCollisionComponent`

**Inheritance:**
```
UActorComponent
  └─ UCombinedBundleActorComponent   — Asset loading framework, GameFeatureName tracking
       └─ UPawnDrivingActorComponent_Base  — PawnOwner / PawnController, possession events
            └─ UInteractCollisionComponent
                 implements IInteractControlInterface   — Interact() / StopInteracting()
                 implements IShapeUpdatedInterface      — ShapeUpdated()
```

**Constructor Defaults:**

| Property | Value | Purpose |
|----------|-------|---------|
| `GameFeatureName` | `"Interact"` | Links asset loading to this feature |
| `bComponentHasComponentAssetToLoad` | `true` | Loads component-level assets (not actor-level) |
| `bHasLimit` | `false` | No limit by default |
| `Limit` | `1` | Default value if a limit is activated |
| `PrimaryComponentTick.bCanEverTick` | `true` | Tick enabled (Editor-only for debug drawing) |

**Transient Runtime Properties:**

| Property | Type | Purpose |
|----------|------|---------|
| `InteractableComponents` | `TArray<UBoxComponent*>` | Created collision boxes |
| `InteractWidgetInstance` | `UUserWidget*` | Currently displayed UI instance |
| `InteractUIImage` | `TSoftObjectPtr<UTexture2D>` | Interaction icon |
| `WidgetTemplate` | `TSoftClassPtr<UUserWidget>` | Loaded widget class |
| `InteractableActors` | `TArray<AActor*>` | All currently overlapping actors |
| `CurrentInteractingActor` | `AActor*` | Most recently interacting actor |
| `bHasLimit` | `bool` | Whether an interaction limit applies |
| `Limit` | `int32` | Remaining interactions |
| `TurnedCollisionOnElement` | `TArray<bool>` | 90° rotation flag per collision box |

**Editor-only Properties:**

| Property | Default | Purpose |
|----------|---------|---------|
| `bDebugMode` | `true` | Draws green debug boxes around all collision components (TickComponent) |
| `bDebugCollisionBoxComponent` | `false` | Makes box components visible in-game |

---

### `UGameFeature_Interact_Settings`

Global configuration. Stored in `DefaultGame.ini` (`Config = Game, defaultconfig`).

| Property | EditCondition | Purpose |
|----------|--------------|---------|
| `bUseGlobalQueryCollisionDebug` | — | Master switch for collision debug |
| `bGlobalQueryCollisionDebug` | `bUseGlobalQueryCollisionDebug` | Enables global collision debug |
| `DefinedCollisionDebug` | — | Actor classes with debug visualization (`HideInDetailPanel`) |
| `bUseGlobalDefaultInteractLimit` | — | Master switch for global interaction limit |
| `bHasDefaultInteractLimit` | `bUseGlobalDefaultInteractLimit` | Whether the global limit is active |
| `DefaultInteractLimit` | `bUseGlobalDefaultInteractLimit` | Global default limit value |

---

### `UInteractUserWidgetBase`

Base widget class for the interaction display.

**Inheritance:** `UUserWidget` + `IInteractWidgetInterface`

| Element | Type | Binding |
|---------|------|---------|
| `Text` | `UTextBlock*` | `BindWidget` — interaction label |
| `Image` | `UImage*` | `BindWidget` — interaction icon |

Implements `IInteractWidgetInterface`:
- `SetImage_Implementation(UTexture2D*)` — sets the icon image
- `SetText_Implementation(const FString&)` — sets the label text

Flags: `DisableNativeTick`, `editinlinenew`, `Blueprintable`

---

### `IInteractWidgetInterface`

Interface between `UInteractCollisionComponent` and the widget.

| Method | Event |
|--------|-------|
| `SetImage(UTexture2D*)` | Set icon (`BlueprintNativeEvent`) |
| `SetText(const FString&)` | Set text (`BlueprintNativeEvent`) |

---

## Asset Loading Types

`SetupLoadedAsset()` distinguishes three Primary Asset Types and responds accordingly:

| Asset Type | Class | Effect |
|------------|-------|--------|
| `"WidgetDefinition"` | `UWidgetDefinitionDataAsset` | Widget class → `WidgetTemplate` |
| `"CollisionBoxConfig"` | `UCollisionBoxConfigDataAsset` | Rotation flag → `TurnedCollisionOnElement`, then `BuildInteractableCollisionComponents()` |
| `"ActorUIImage"` | `UActorUIImageDataAsset` | Texture → `InteractUIImage` |

---

## Flow

### Initialization

```
[Assets loaded by UCombinedBundleActorComponent]
SetupLoadedAsset() — called repeatedly, once per asset:

  PrimaryAssetType == "WidgetDefinition"
    → load WidgetClass synchronously
    → WidgetTemplate = TSoftClassPtr(WidgetClass)

  PrimaryAssetType == "CollisionBoxConfig"
    → bTurned90Degrees? → TurnedCollisionOnElement.Add(true/false)
    → BuildInteractableCollisionComponents()
         → CreateBoxComponent(RootComponent, 0)
              → ISceneComponentCatchInterface::GetSceneComponent(Owner, "Mesh")
              → Bounds = MeshComponent->GetLocalBounds()
              → NewObject<UBoxComponent>
              → AttachToComponent(RootComponent)
              → SetBoxExtent(Bounds.BoxExtent)
              → SetRelativeLocation(0, 0, BoxExtent.Z)
              → Collision: QueryOnly · ECC_GameTraceChannel2 · ECR_Overlap(ECC_Pawn)
              → BindDynamic: OnBoxComponentBeginOverlap / OnBoxComponentEndOverlap
              → InteractableComponents.Add(BoxComponent)

  PrimaryAssetType == "ActorUIImage"
    → load image synchronously
    → InteractUIImage = TSoftObjectPtr(Texture)
```

### Runtime — Overlap Events

```
OnBoxComponentBeginOverlap(OtherActor)
  ├─ OtherActor == Owner  →  return
  ├─ InstigatorController == null  →  return
  ├─ !CheckLimit()  →  return (limit reached)
  ├─ InstigatorController == FirstPlayerController
  │    └─ ShowWidget()
  │         ├─ WidgetTemplate.LoadSynchronous() (if needed)
  │         ├─ CreateWidget → AddToViewport()
  │         ├─ IInteractWidgetInterface::SetImage(InteractUIImage)
  │         └─ IInteractWidgetInterface::SetText("Interact with {OwnerName}")
  ├─ IRegisterComponentInterface::RegisterComponentWithInterface(Controller, this)
  ├─ CheckSenseTouch(OtherActor, true)
  │    └─ IHumanoidSenseInterface::RegisterObject(OtherActor, GetOwner(), ST_TOUCH)
  ├─ CheckSenseVisual(OtherActor, true)
  │    └─ IHumanoidSenseInterface::RegisterObject(OtherActor, GetOwner(), ST_VISUAL)
  └─ InteractableActors.AddUnique(OtherActor)

OnBoxComponentEndOverlap(OtherActor)
  ├─ InstigatorController == FirstPlayerController
  │    └─ RemoveWidget()  →  InteractWidgetInstance->RemoveFromParent()
  ├─ IRegisterComponentInterface::UnregisterComponentWithInterface(Controller, this)
  ├─ CheckSenseTouch(OtherActor, false)
  ├─ CheckSenseVisual(OtherActor, false)
  └─ InteractableActors.Remove(OtherActor)
```

### Interact Input

```
Interact_Implementation(InstigatorController)
  ├─ Owner must implement IInteractInterface
  └─ IInteractInterface::GetOnInteractWithPawn()
       .Broadcast(InstigatorController->GetPawn(), InstigatorController)
```

`ACombinedBundleActor/Pawn/Character` and `CombinedWheeledVehiclePawn` receive the broadcast via their `FOnInteractWithPawn` delegate binding and delegate to `InteractManageGameInstanceSubsystem` for pawn swapping.

---

## Interaction Limit

`CheckLimit()` returns `true` when:
- `bHasLimit == false` (no limit set), **or**
- `bHasLimit == true` and `Limit > 0`

The global default limit can be configured via `UGameFeature_Interact_Settings`.

---

## Lifecycle Events

| Event | Behavior |
|-------|----------|
| `BeginPlay` | Checks `IsGameFeatureIgnored()`, logs start |
| `ProcessPossess` | Removes the widget when the first player possesses the owner |
| `EndPlay` | Removes the widget regardless of reason |
| `ShapeUpdated_Implementation` | Updates `InteractableComponents[0]` with new mesh bounds |

---

## Requirements for the Owning Actor

| Interface | Slot | Purpose |
|-----------|------|---------|
| `ISceneComponentCatchInterface` | `"Mesh"` | Provides the mesh component for collision box bounds calculation |
| `IInteractInterface` | — | Receives `FOnInteractWithPawn` broadcasts |

The controller of the interacting pawn **should** implement `IRegisterComponentInterface` so the component can register and unregister with the controller.

---

## CoreInformation Editor Integration

`GetSettingsVariableSets_Implementation()` provides three `FInformationSet` groups:

| Group | Contents |
|-------|----------|
| `"Interact"` | `HasLimit` (bool), `Limit` (int, only when `bHasLimit == true`) |
| `"Current Interacting Actor"` | Name of `CurrentInteractingActor` or `"No actor is currently interacting"` |
| `"Interactable Actors"` | Names of all actors in `InteractableActors` or `"No actors found"` |

---

## Logging

| Category | Contents |
|----------|----------|
| `Log_Interact` | General lifecycle, overlap events, limit checks |
| `Log_Interact_Debug` | Editor tick — debug collision box visibility |
| `Log_Interact_Setup` | Asset loading, box creation, ShapeUpdated |
| `Log_Interact_Widget` | Widget creation, display, removal |
| `Log_Interact_Class` | Class-specific events |

All logs use `UE_LOGFMT` with fields `Time`, `Line`, `Class`.

---

## Module Dependencies

### `GameFeature_InteractRuntime`

| Module | Visibility | Reason |
|--------|------------|--------|
| `Core` | Public | UE base types |
| `GameFeature_InteractSetup` | Public | Settings class accessible from the Runtime module |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard UE runtime |
| `UMG` | Private | Widget system (`UUserWidget`, `AddToViewport`) |
| `AssetLoader` | Private | Asset Manager integration |
| `Core_Classes` | Private | `UPawnDrivingActorComponent_Base` |
| `Elemental_Classes` | Private | Base actor/pawn classes |
| `Elemental_Data` | Private | Data types from Elemental_Collection |
| `Elemental_Interfaces` | Private | All used interfaces |
| `Elemental_Logging` | Private | `GlobalLog`, `StructuredLog` |
| `Elemental_Structure` | Private | Shared structs (`FInformationSet`) |
| `Core_Interfaces` | Private | `IRegisterComponentInterface` and others |

### `GameFeature_InteractSetup`

| Module | Visibility | Reason |
|--------|------------|--------|
| `Core` | Public | UE base types |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard UE runtime |
