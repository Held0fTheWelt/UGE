# Helicopters Plugin

Provides the full helicopter framework: actor and pawn base classes in Static-Mesh and Skeletal-Mesh variants, a physics-driven flight model, data-driven flight presets, a dual-path animation system, and a set of interfaces for controls, engine state, SAS, auto-hover, and auto-trim.

## Module

| Module | Type | Loading Phase |
|--------|------|---------------|
| `Helicopters` | Runtime | Default |

## Architecture

The plugin provides two parallel hierarchies depending on how the helicopter is used in the world:

```
ACombinedBundleActor  (Core_Classes)           ACombinedBundlePawn  (Core_Classes)
  └─ AHelicopterActor                            └─ AHelicopterPawn  ← PCT_HELICOPTER
       ├─ ASM_HelicopterActor  (StaticMesh)           ├─ ASM_HelicopterPawn  (StaticMesh)
       └─ ASKM_HelicopterActor (SkeletalMesh)          └─ ASKM_HelicopterPawn (SkeletalMesh)
```

**Actors** are world-placed, non-possessable helicopters (parked, props, AI-driven scene objects).
**Pawns** are possessable by a player controller. `GetPawnControlType_Implementation()` returns `EPawnControlType::PCT_HELICOPTER`, which causes the `Controllers` plugin to bind the helicopter input action set.

**SM variants** use a `UStaticMeshComponent` as root — suitable for rigid, non-animated helicopters.
**SKM variants** use a `USkeletalMeshComponent` as root and support async asset loading via `SetupLoadedAsset()` (see below).

## Classes

### Actors

| Class | Base | Mesh type | Notes |
|-------|------|-----------|-------|
| `AHelicopterActor` | `ACombinedBundleActor` | — | Tick-enabled base |
| `ASM_HelicopterActor` | `AHelicopterActor` | `UStaticMeshComponent` | Routes `GetSceneComponent("Mesh"/"Collision")` to its mesh |
| `ASKM_HelicopterActor` | `AHelicopterActor` | `USkeletalMeshComponent` | Async mesh+anim loading; notifies `IShapeUpdatedInterface` |

### Pawns

| Class | Base | Mesh type | Notes |
|-------|------|-----------|-------|
| `AHelicopterPawn` | `ACombinedBundlePawn` | — | `PCT_HELICOPTER`, Tick-enabled |
| `ASM_HelicopterPawn` | `AHelicopterPawn` | `UStaticMeshComponent` | Routes `GetSceneComponent` |
| `ASKM_HelicopterPawn` | `AHelicopterPawn` | `USkeletalMeshComponent` | Async mesh+anim loading; notifies `IShapeUpdatedInterface` |

### SKM Asset Loading (`SetupLoadedAsset`)

Both `ASKM_*` variants override `SetupLoadedAsset(UObject* Owner, UObject* LoadedObject)`:

1. Validates owner identity (`Cast<AActor>(InOwner) == this`).
2. Checks that `LoadedObject` implements `UAssetTypeInterface`.
3. Dispatches on `PrimaryAssetType`: if `UMeshSetupDataAsset` → casts to `USkeletalMeshSetupDataAsset`.
4. Calls `SetMeshAndAnimInstance(Mesh, AnimClass)` which sets the skeletal mesh and assigns the Animation Blueprint.
5. Broadcasts `IShapeUpdatedInterface::Execute_ShapeUpdated` to all registered components so collision shapes can rebind to the new mesh geometry.

## Interfaces

### `IHelicopterControlsInterface`

Exposes four multicast delegates for current pilot input values. Implemented by the helicopter control component.

| Delegate | Payload | Description |
|----------|---------|-------------|
| `FNotifyCurrentCollective` | `float` | Collective stick position |
| `FNotifyCurrentCyclic` | `FVector2D` | Cyclic stick (pitch/roll) |
| `FNotifyCurrentPedals` | `float` | Tail rotor pedals (yaw) |
| `FNotifyCurrentThrottle` | `float` | Engine throttle |

### `IHelicopterEngineInterface`

Nine delegates broadcasting live physics state from the flight simulation.

| Delegate | Payload |
|----------|---------|
| `FNotifyCurrentMainRotorSpeed` | `float` |
| `FNotifyCurrentTailRotorSpeed` | `float` |
| `FNotifyCurrentLiftValue` | `float` |
| `FNotifyEngineThrustValue` | `float` |
| `FNotifyCurrentSideForceValue` | `float` |
| `FNotifyCurrentLinearDamping` | `float` |
| `FNotifyCurrentAirFriction` | `float` |
| `FNotifyTorqueReductionSpeed` | `float` |
| `FNotifyCurrentPhysicsLiftVector` | `FVector` |

### `IHelicopterPhysicsInterface`

Single delegate for aileron angle, consumed by the animation system.

| Delegate | Payload |
|----------|---------|
| `FNotifyCurrentAileronAngle` | `float` |

### `IHelicopterSASInterface` *(BlueprintNativeEvent)*

Interface for the Stability Augmentation System. Implemented by `SASHelicopterComponent`, which lives in the `GameFeature_StabilityAugmentedSystem` feature. Called by the helicopter control component on the SAS toggle key.

```cpp
void ToggleSAS();
bool IsSASEnabled() const;
EHeliSASHoldMode GetSASHoldMode() const;
```

`FNotifySASStateChanged(bool bEnabled, EHeliSASHoldMode HoldMode)` is broadcast on state change.

### `IHelicopterAnimInstanceInterface`

Two delegates wiring the engine simulation to the AnimInstance.

| Delegate | Payload |
|----------|---------|
| `FNotifyCurrentRotorMain` | `float` |
| `FNotifyCurrentRotorTail` | `float` |

### `IHelicopterCollectiveOffsetProviderInterface` *(Auto-Hover, P6.2)*

Implemented by an auto-hover component. The control component calls these before summing the final collective command.

```cpp
// Informs the provider of the current raw pilot input (for override detection)
void NotifyPilotCollective(float PilotCollective);

// Returns PID offset to add to pilot collective (0 when inactive or overridden)
float GetCollectiveOffset() const;
```

### `IHelicopterTrimNotifyInterface` *(Auto-Trim, P6.3)*

Called when the cyclic stick is released so a trim component can capture the current attitude as the new trim target.

```cpp
void NotifyStickReleasedForTrim();
```

## Data Assets

### `UHeliFlightPresetDataAsset`

Holds one `FHeliModePreset` that defines all physics parameters for a given flight mode. Inherits from `UPreLoadingDataAsset` (async-loadable).

**Flight modes** (`EHeliFlightMode`): `Realistic`, `SAS_Assist`, `Arcade`
**SAS hold modes** (`EHeliSASHoldMode`): `Off`, `LevelHold`, `TrimHold`

`FHeliModePreset` bundles:

| Sub-struct | Key properties |
|------------|----------------|
| `FHeliDiscTiltConfig` | `MaxTiltDeg` (14°), `TiltRateDegPerSec` (60) |
| `FHeliDragConfig` | `LinearCoeff`, `QuadraticCoeff` |
| `FHeliDampingConfig` | `LinearDamping`, `AngularDamping`, optional extra values for Arcade |
| `FHeliSASConfig` | Rate damping, attitude hold, yaw damper; deadzones, blend times, PID gains per axis, max assist torque |
| `FHeliArcadeConfig` | Helper forces toggle, `AssistStrength`, per-axis force scales |
| `FHeliCyclicConfig` | Optional pitch/roll torque gain override, `CyclicIdleThrust` |
| `FHeliCOMConfig` | Optional center-of-mass offset override (P0.5) |
| `FHeliMainRotorTorqueConfig` | Reactive yaw torque scale, rotor direction sign (±1 for US/EU vs Russian convention), tail rotor lateral force scale |

### `UPhysicsBasedHelicopterDefinitionDataAsset`

Defines the skeletal rig layout and physics tuning for a specific helicopter type.

**Bone mapping:**

| Property | Type | Purpose |
|----------|------|---------|
| `Body` | `FHelicopterPhysicsBone` | Main physics body bone |
| `MainRotorBone` | `FHelicopterPhysicsBone` | Rotor shaft drive bone |
| `TailRotorBone` | `FHelicopterPhysicsBone` | Tail rotor drive bone |
| `MainRotorBoneNames` | `TArray<FName>` | All spinning main rotor blades |
| `TailRotorBoneNames` | `TArray<FName>` | All spinning tail rotor blades |
| `ClocheBoneNames` | `TArray<FName>` | Swashplate/cloché bones |
| `AileronBoneNames` | `TArray<FName>` | Control surface bones |
| `BoneName_Rotor_Angle_X/Y` | `FName` | Disc tilt control bones |

**Physics parameters:**

| Property | Default | Description |
|----------|---------|-------------|
| `TorqueMultiplier` | 24 | Yaw angular acceleration (deg/s², mass-independent) |
| `MaxAngularVelocityDeg` | 90 | Runaway rotation cap (deg/s) |
| `CyclicPitchTorqueGain` | 50 | Nm per degree of disc tilt (pitch) |
| `CyclicRollTorqueGain` | 50 | Nm per degree of disc tilt (roll) |
| `CyclicIdleThrust` | 100 000 N | Minimum thrust when disc is tilted without collective |
| `Rotor_Rotation_Scalar` | −360 | Visual rotor spin rate (deg/s per normalised RPM) |
| `OverrideMass` | 0 | kg; 0 = use mesh physics mass |
| `bInvertRoll/Pitch/Yaw` | false | Calibration: reverse axis response |
| `bSwapCyclicXY` | false | Calibration: swap cyclic axes |

Specification fields (`MainRotorDiameter_cm`, `TailRotorDiameter_cm`, `Length_cm`) are documentation-only and not used in physics calculations.

## Animation

### `UHelicopterAnimInstance`

Dual-path AnimInstance inheriting from both `UAnimInstance` and `IHelicopterAnimInstanceInterface`.

**Path 1 — Legacy delegate-based** (any existing component)
`FNotifyCurrentRotorMain/Tail` delegates feed `Rotor_Main` and `Rotor_Tail` floats used by legacy AnimBP nodes.

**Path 2 — SKM push-based** (`SKM_PhysicsBasedHelicopterComponent` calls setters each tick)

Setters:
```cpp
SetMainRotorSpeed(float)
SetTailRotorSpeed(float)
SetCollectiveInput(float)
SetCalibratedCyclic(FVector2D)
SetCalibratedPedals(float)
SetDiscTilt(float Pitch, float Roll)
SetAnimationScalars(float RotorScalar, float MF, float MR, float Pedals)
```

`NativeUpdateAnimation` computes these BlueprintReadOnly outputs for AnimBP "Modify Bone" nodes:

| Output | Description |
|--------|-------------|
| `MainRotorYaw` | Accumulated main rotor spin angle |
| `TailRotorPitch` | Accumulated tail rotor spin angle |
| `MainBladeCollectiveRoll` | Blade pitch from collective input |
| `TailBladePedalRoll` | Tail blade pitch from pedal input |
| `ClocheRotation` | Full swashplate FRotator |
| `AileronPitchAngle` | Control surface deflection |
| `DiscAnglePitch/Roll` | Current disc tilt angles |

### `UHelicopterWTurretAnimInstance`

Empty subclass of `UHelicopterAnimInstance`. Reserved as the AnimInstance class for helicopters equipped with a turret, allowing turret-specific animation variables to be added without touching the base class.

## Structs & Enums

| Type | Values / Fields |
|------|-----------------|
| `EHelicopterPhysicalComponentType` | `HPT_NONE`, `HPT_BODY`, `HPT_ROTOR_SHAFT`, `HPT_ROTOR_TAIL` |
| `FHelicopterPhysicsBone` | `ComponentType`, `ComponentBoneName` |
| `FAutoHoverSettings` | `bAutoHoveringEnabled`, `bMaintainAltitude`, `bMaintainVerticalSpeed`, `bMaintainLocation`, `bMaintainYaw`, `bMaintainHorizontalSpeed` |

## Logging

| Category | Location |
|----------|----------|
| `Log_HelicopterActor` | `SKM_HelicopterActor.cpp` |
| `Log_SKMHelicopterPawn` | `SKM_HelicopterPawn.cpp` |

## Dependencies

### Plugin dependencies

| Plugin | Role |
|--------|------|
| `Core_Collection` | `ACombinedBundleActor/Pawn`, `EPawnControlType`, `UPreLoadingDataAsset` |
| `Elemental_Collection` | Shared interfaces and data types |

### Module dependencies

| Module | Visibility |
|--------|-----------|
| `Core` | Public |
| `CoreUObject`, `Engine`, `Slate`, `SlateCore` | Private |
| `Core_Classes` | Private — `ACombinedBundleActor/Pawn` |
| `Core_Data` | Private — `UPreLoadingDataAsset` |
| `Core_Interfaces` | Private — `IAssetTypeInterface`, `IShapeUpdatedInterface` |
| `Elemental_Classes` | Private |
| `Elemental_Data` | Private — `USkeletalMeshSetupDataAsset`, `UMeshSetupDataAsset` |
| `Elemental_Interfaces` | Private |
| `Elemental_Logging` | Private — `GlobalLog`, `StructuredLog` |
| `AssetLoader` | Private — async soft-ref loading |

## Extension pattern

Create a Blueprint or C++ subclass of `ASKM_HelicopterPawn` (or Actor) and:

1. Assign a `UPhysicsBasedHelicopterDefinitionDataAsset` to configure the rig bone layout and physics tuning.
2. Assign one or more `UHeliFlightPresetDataAsset` entries for each desired flight mode (Realistic / SAS_Assist / Arcade).
3. Add a `SKM_PhysicsBasedHelicopterComponent` (from a Game Feature or directly) — it reads both data assets, drives the physics tick, and pushes values into `UHelicopterAnimInstance` each frame via the setters.
4. Optionally add `SASHelicopterComponent` (implements `IHelicopterSASInterface`) and an auto-hover component (implements `IHelicopterCollectiveOffsetProviderInterface`) as Game Feature actions.
