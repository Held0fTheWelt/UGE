# AssetLoader

A runtime plugin for data-driven, asynchronous asset loading. It decouples asset consumers from asset discovery by routing all loading through 
**Primary Data Assets**, **DataTables**, and a two-tier definition hierarchy.

---

## Core Concept

Loading is split into two layers:

```
PrimaryDataAssetSet
  └── UPreLoadingDataAsset (one or more)
        ├── GetAssetClasses() → TSoftClassPtr[]
        └── GetAssetObjects() → TSoftObjectPtr[]
```

The `UPrimaryDataAssetSet` is the entry point. It groups a collection of `UPreLoadingDataAsset` instances. Each `UPreLoadingDataAsset` 
exposes the concrete soft references that ultimately get streamed in. The plugin handles everything in between: async loads, GC protection, and event broadcasting.

---

## Asset Definition Types

`EAssetDefinitionType` controls which loading path is taken:

| Value | Description |
|---|---|
| `ADT_ACTOR` | Actor class definitions via `UActorDefinitionPrimaryDataAsset` |
| `ADT_COMPONENT` | Component class definitions via `UComponentDefinitionPrimaryDataAsset` |
| `ADT_INPUT` | Input configs — discovered by type via AssetManager, not by name |
| `ADT_OTHER` | Custom definitions |

---

## Setup

### 1. Create a Pre-Loading Data Asset

Inherit from `UPreLoadingDataAsset` and implement the `IPreLoadingAssetInterface`:

- `GetAssetClasses()` — return soft class references to load
- `GetAssetObjects()` — return soft object references to load

### 2. Create a Primary Data Asset Set

Create a `UPrimaryDataAssetSet` asset and add your `UPreLoadingDataAsset` instances to it. The asset must be named using the convention:

```
DA_PrimaryDataSet_<DefinitionName>
```

The `<DefinitionName>` is what you pass as the definition name when triggering a load.

### 3. Register with the Asset Manager

All Primary Data Assets must be registered in `DefaultGame.ini` under `[/Script/Engine.AssetManagerSettings]` so the Asset Manager can discover them at runtime.

### 4. (Optional) Actor/Component Definitions

For `ADT_ACTOR` or `ADT_COMPONENT`, create a `UActorDefinitionPrimaryDataAsset` or `UComponentDefinitionPrimaryDataAsset` pointing to a `UDataTable` 
(row type `FActorDefinitionTable` / `FComponentDefinitionTable`). Each row maps a `FName` to a set of definition data assets via `FActorAndComponentSelector`.

---

## Loading

### C++ — Definition-based loading

```cpp
FAssetLoaderEvents Events;

Events.OnPrimaryAssetLoaded.BindUObject(this, &UMyClass::OnAssetLoaded);
Events.OnPrimaryAssetClassNotFound.BindUObject(this, &UMyClass::OnAssetFailed);

UAssetManagedLoaderHelper::LoadDefinitionConfigAssets(
    Events,
    EAssetDefinitionType::ADT_ACTOR,
    this,               // Owner
    false,              // bComponentHasComponentAssetToLoad
    true,               // bComponentHasActorAssetToLoad
    false               // bDebugRoute
);
```

Callbacks:

```cpp
void UMyClass::OnAssetLoaded(UObject* Owner, UObject* LoadedObject) { ... }
void UMyClass::OnAssetFailed(UObject* Owner, bool bHasClassesToLoad) { ... }
```

### C++ / Blueprint — Direct async loading

```cpp
UAsyncAssetLoaderHelper* Loader = UAssetManagedLoaderHelper::LoadObjectsAndClassesAsync(
    this,
    ObjectSoftRefs,
    ClassSoftRefs
);
Loader->OnAllAssetsLoaded.AddDynamic(this, &UMyClass::OnLoaded);
Loader->StartLoading();
```

### C++ / Blueprint — Input config loading

```cpp
UControllerAsyncAssetLoaderHelper* Loader = UAssetManagedLoaderHelper::LoadInputAssetsAsync(
    this,
    ObjectSoftRefs,
    ClassSoftRefs,
    InputConfigType
);
Loader->OnAllAssetsLoaded.AddDynamic(this, &UMyClass::OnInputLoaded);
Loader->StartLoading();
```

For `ADT_INPUT`, `LoadDefinitionConfigAssets` discovers all `UInputConfigPrimaryDataAssetSet` assets automatically via the Asset Manager — no definition name is needed.

---

## Plugin Settings

Configurable under **Project Settings → Asset Loader** (`DefaultGame.ini`, section `[Asset Loader]`):

| Property | Default | Description |
|---|---|---|
| `bUsePrimaryActorDefiningDataAsset` | `false` | Use a primary actor-defining data asset as the top-level lookup |
| `PrimaryDataAssetIdentifier` | `"GlobalDefinitions"` | Fallback identifier used when no explicit definition name is given |
| `PrimaryEntityDefinitionLookupTable` | *(none)* | Soft reference to the global `UActorDefinitionPrimaryDataAsset` |

---

## Key Classes

| Class | Role |
|---|---|
| `UAssetManagedLoaderHelper` | Static entry point for all loading operations |
| `UAsyncAssetLoaderHelper` | Manages a single async load batch (objects + classes) |
| `UControllerAsyncAssetLoaderHelper` | Like above, but carries an `InputType` through the callback |
| `UPrimaryDataAssetSet` | Groups `UPreLoadingDataAsset` instances into a named set |
| `UPreLoadingDataAsset` | Abstract base — override to expose your soft references |
| `UInformativePrimaryDataAsset` | Abstract base for custom primary data assets with flexible type naming |
| `UActorDefinitionPrimaryDataAsset` | Wraps a DataTable of actor definitions |
| `UComponentDefinitionPrimaryDataAsset` | Wraps a DataTable of component definitions |
| `UInputConfigPrimaryDataAssetSet` | Base class for input configuration asset sets |
| `FAssetLoaderEvents` | Holds the two load result delegates |

---

## Interfaces

| Interface | Purpose |
|---|---|
| `IPreLoadingAssetInterface` | Expose `GetAssetClasses()` / `GetAssetObjects()` on a data asset |
| `IAssetDefinitionsInterface` | Expose `GetAssetNames()` from a definition container |
| `IAssetDefinitionInformationInterface` | Get/set the definition asset name on a runtime object |
| `IAssetTypeInterface` | Expose `GetAssetType()` returning a `FPrimaryAssetType` |

---

## Dependencies

- **LoggingMacros** plugin (required)
- Engine modules: `Core`, `CoreUObject`, `Engine`, `AssetRegistry`, `Slate`, `SlateCore`
