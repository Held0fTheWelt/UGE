# InputSystem

A runtime plugin that extends Unreal's Enhanced Input System with a type-safe, data-driven configuration layer. It supports multiple simultaneous entity types (humanoids, vehicles, aircraft, helicopters, turrets, weapons) and hardware profiles (keyboard/gamepad, steering wheel) through metadata-annotated assets and a central primary data asset.

---

## Core Concept

The plugin wraps two Enhanced Input classes with additional metadata and bundles them into a discoverable primary data asset:

```
UInputConfigDataAssetSet  (per entity type)
  ├── EInputSettingType             → which entity this profile is for
  ├── EInputMappingType             → which hardware profile (default / steering wheel)
  ├── EInputConfiguration           → which configuration slot (1–10)
  ├── UEnhancedInputMappingContext[] → key/button mappings
  └── UEnhancedInputAction[]        → individual actions with semantic type tags
```

At runtime, consumers query the profile via `IInputAssetInterface`, load the soft references, and apply the mapping contexts to the Enhanced Input subsystem. Which configuration variant to load is determined via `IInputControllerInterface` on the owning actor.

---

## Dependencies

| Dependency | Type |
|---|---|
| `EnhancedInput` | Unreal Engine plugin |
| `AssetLoader` | Custom plugin — provides `UInputConfigPrimaryDataAssetSet` |
| `LoggingMacros` | Custom plugin — structured logging |

---

## Asset Setup

### 1. Create Input Actions

Create assets of type **`UEnhancedInputAction`** (inherits `UInputAction`). Set the two metadata fields:

| Property | Type | Purpose |
|---|---|---|
| `InputPawnTypes` | `TArray<EInputSettingType>` | Which entity types use this action |
| `InputActionType` | `EInputActionType` | Semantic tag (e.g. `IAT_MOVE`, `IAT_SHOOT`) |

### 2. Create Mapping Contexts

Create assets of type **`UEnhancedInputMappingContext`** (inherits `UInputMappingContext`). Add key bindings to your `UEnhancedInputAction` assets as usual, then fill the metadata:

| Property | Type | Purpose |
|---|---|---|
| `InputPawnTypes` | `TArray<EInputSettingType>` | Which entity types use this context |
| `InputMappingType` | `EInputMappingType` | Hardware profile (`IMT_DEFAULT` or `IMT_STEERING_WHEEL`) |
| `InputConfiguration` | `EInputConfiguration` | Configuration slot (`IC_CONFIGURATION_1` … `IC_CONFIGURATION_10`) |

### 3. Create a Config Data Asset Set

Create an asset of type **`UInputConfigDataAssetSet`** for each entity type + hardware combination:

| Property | Type | Purpose |
|---|---|---|
| `InputSettingType` | `EInputSettingType` | Target entity type |
| `InputSettingIdentifier` | `FString` | Unique profile identifier |
| `InputTypes` | `TArray<EInputSettingType>` | Supported types |
| `InputMappingContexts` | `TArray<TSoftObjectPtr<UEnhancedInputMappingContext>>` | Contexts for this profile |
| `InputMappingActions` | `TArray<TSoftObjectPtr<UEnhancedInputAction>>` | Actions for this profile |

Before use, call `IsReadyForUse()` (Blueprint callable) to verify the asset is fully configured.

### 4. Register with AssetLoader

`UInputConfigDataAssetSet` inherits from `UInputConfigPrimaryDataAssetSet` (from the AssetLoader plugin). Register it in `DefaultGame.ini` under `[/Script/Engine.AssetManagerSettings]` so the Asset Manager can discover it. Pass `ADT_INPUT` to `UAssetManagedLoaderHelper::LoadDefinitionConfigAssets()` — it will find all registered instances automatically without requiring a definition name.

---

## Runtime Usage

### Implementing `IInputControllerInterface`

Add this to any actor that can switch input configurations (e.g. a PlayerController):

```cpp
class AMyController : public APlayerController, public IInputControllerInterface
{
    virtual EInputConfiguration GetInputConfiguration_Implementation() const override
    {
        return CurrentConfiguration; // IC_CONFIGURATION_1, IC_CONFIGURATION_2, etc.
    }
};
```

### Querying a Config Asset (C++)

```cpp
// Cast to interface and query
if (UObject* Asset = /* loaded UInputConfigDataAssetSet */)
{
    EInputSettingType Type = IInputAssetInterface::Execute_GetInputSettingType(Asset);
    TArray<FSoftObjectPath> Contexts = IInputAssetInterface::Execute_GetInputMappingContexts(Asset);
    TArray<FSoftObjectPath> Actions  = IInputAssetInterface::Execute_GetInputActions(Asset);

    // Stream Contexts, then add to Enhanced Input subsystem
}
```

### Validating before use

```cpp
if (UInputConfigDataAssetSet* Profile = /* ... */)
{
    if (Profile->IsReadyForUse())
    {
        // safe to load and apply
    }
}
```

---

## Interfaces

### `IInputAssetInterface`

Implement on any class that exposes an input configuration profile.

| Method | Return | Description |
|---|---|---|
| `GetInputSettingType()` | `EInputSettingType` | The entity type this profile targets |
| `GetInputMappingContexts()` | `TArray<FSoftObjectPath>` | Soft paths to the mapping contexts |
| `GetInputActions()` | `TArray<FSoftObjectPath>` | Soft paths to the input actions |

### `IInputControllerInterface`

Implement on actors/controllers that need to select a configuration slot.

| Method | Return | Description |
|---|---|---|
| `GetInputConfiguration()` | `EInputConfiguration` | The active configuration slot |

Both interfaces expose `BlueprintNativeEvent` — override in C++ or Blueprint.

---

## Enumerations

### `EInputSettingType` — Entity type

| Value | Entity |
|---|---|
| `IST_NONE` | Unset |
| `IST_HUMANOID` | Biped characters |
| `IST_VEHICLE` | Ground vehicles |
| `IST_TURRET` | Stationary turrets |
| `IST_AIRCRAFT` | Fixed-wing aircraft |
| `IST_HELICOPTER` | Rotorcraft |
| `IST_WEAPON` | Weapon-specific controls |
| `IST_INTERACT` | Interaction mechanics |
| `IST_CONTROL_ENGINE` | Engine throttle/control |
| `IST_CONTROLS_ENGINE` | Engine-related actions |
| `IST_CONTROL_CAMERA` | Camera pan / zoom / rotate |
| `IST_CONTROL_SYSTEMS` | Internal system controls |

### `EInputActionType` — Semantic action tag

| Group | Values |
|---|---|
| General | `IAT_LOOK`, `IAT_TOGGLECAMERA`, `IAT_ZOOMCAMERA`, `IAT_INTERACT`, `IAT_JUMP`, `IAT_MOVE`, `IAT_RUN`, `IAT_RESET`, `IAT_SHOOT`, `IAT_ROTATE` |
| Ground vehicle | `IAT_THROTTLE`, `IAT_THROTTLE_KEYUP`, `IAT_THROTTLE_KEYDOWN`, `IAT_STEER`, `IAT_BRAKE`, `IAT_HANDBRAKE` |
| Aircraft / Helicopter | `IAT_COLLECTIVE`, `IAT_COLLECTIVE_KEY`, `IAT_CYCLIC`, `IAT_CYCLIC_KEY_W/A/S/D`, `IAT_PEDALS`, `IAT_PEDALS_KEY_LEFT/RIGHT` |
| Systems | `IAT_ENGINE_SWITCH`, `IAT_ENGINE_2ND_SWITCH`, `IAT_SAS_TOGGLE` |
| Reserved | `IAT_PLACEHOLDER` |

### `EInputMappingType` — Hardware profile

| Value | Device |
|---|---|
| `IMT_NONE` | Unset |
| `IMT_DEFAULT` | Keyboard, mouse, gamepad |
| `IMT_STEERING_WHEEL` | Steering wheel peripheral |

### `EInputConfiguration` — Configuration slot

`IC_NONE`, `IC_CONFIGURATION_1` … `IC_CONFIGURATION_10`

---

## Project Settings

Accessible under **Project Settings → War Collection → Input Control Settings** (registered at module startup, serialized to `DefaultGame.ini`).

| Property | Type | Description |
|---|---|---|
| `InputSettings` | `TArray<FInputData>` | Array of input configuration entries |

`FInputData` (also usable as a DataTable row via `FTableRowBase`):

| Field | Type | Description |
|---|---|---|
| `InputSettingType` | `EInputSettingType` | Entity type for this entry |
| `InputDataIdentifier` | `FString` | Unique identifier (ObjectPath, GUID, or custom string) |
