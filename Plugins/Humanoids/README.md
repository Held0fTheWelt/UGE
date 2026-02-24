# Humanoids Plugin

Provides the concrete `AHumanoidCharacter` class — the bipedal, player-controlled character type used throughout the project.

## Module

| Module | Type | Loading Phase |
|--------|------|---------------|
| `Humanoids` | Runtime | Default |

## Architecture

The plugin sits at the top of the character inheritance chain and provides a single, concrete actor class:

```
ACoreCharacter  (Core_Collection → Core_Classes)
  └─ ACombinedBundleCharacter  (Core_Collection → Core_Classes)
       └─ AHumanoidCharacter  ← this plugin
```

`ACombinedBundleCharacter` bundles the component/interface registration system and the GAS-driven ability bundle system. `AHumanoidCharacter` specialises that foundation for the humanoid form factor.

## Class: `AHumanoidCharacter`

**Header:** `Source/Humanoids/Public/Characters/HumanoidCharacter.h`

### Constructor defaults

| Property | Value | Notes |
|----------|-------|-------|
| Capsule radius | 55 cm | Standard standing human width |
| Capsule half-height | 96 cm | Standard standing human height |
| Collision enabled | `QueryAndPhysics` | Full physics + query |
| `ECC_GameTraceChannel2` response | `ECR_Overlap` | Custom "QueryCollision" channel used by the interact system |
| `bSetMeshToDefaultSkeletalMeshComponent` | `false` | Mesh assignment is deferred to child Blueprints |

Overlap on `ECC_GameTraceChannel2` is what makes the character detectable by proximity-based interaction and sense queries.

### `GetPawnControlType_Implementation()`

Returns `EPawnControlType::PCT_HUMANOID`.

The `Controllers` plugin uses this return value to determine which input action set and which movement-control dispatch path to activate when the controller possesses this pawn. Setting it here ensures every Blueprint subclass automatically inherits the correct controller binding without additional configuration.

### Included interfaces (via the CPP translation unit)

These headers are included in the `.cpp` and are therefore relevant to the runtime behaviour even though they are not explicitly declared in the class body — they are used by the parent class pipeline:

| Interface | Purpose |
|-----------|---------|
| `IInteractInterface` | Marks the character as an interactable target |
| `IHumanoidSenseInterface` | Allows sense queries (touch/proximity detection) from the controller |

## Dependencies

### Plugin dependencies

| Plugin | Role |
|--------|------|
| `Core_Collection` | Base character classes, pawn control type enum, GAS integration |
| `Elemental_Collection` | Shared interfaces and data types |
| `AssetLoader` | Async soft-reference asset loading |

### Module dependencies

| Module | Visibility |
|--------|-----------|
| `Core` | Public |
| `CoreUObject`, `Engine`, `Slate`, `SlateCore` | Private |
| `Core_Classes` | Private — `ACombinedBundleCharacter`, `EPawnControlType` |
| `Elemental_Classes` | Private — shared elemental character base |
| `Elemental_Data` | Private — data assets |
| `Elemental_Interfaces` | Private — interface declarations |
| `Elemental_Logging` | Private — log utilities |
| `AssetLoader` | Private — asset loading helpers |

## Extension pattern

Create Blueprint or C++ subclasses of `AHumanoidCharacter` to add:

- Skeletal mesh and animation Blueprint assignments
- Game Feature–provided ability sets (via `Core_GameFeatures` / `Core_AbilitySystem`)
- Additional component registrations (input, inventory, equipment, etc.)

The base class intentionally leaves `bSetMeshToDefaultSkeletalMeshComponent = false` so that each concrete character variant can assign its own mesh without being overridden by the framework's default assignment logic.
