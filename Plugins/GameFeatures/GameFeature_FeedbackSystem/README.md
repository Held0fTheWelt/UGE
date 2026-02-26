# GameFeature_FeedbackSystem

Feedback system. Provides two independent sub-systems: **Context Effects** (tag-driven, surface-aware VFX and audio triggered from animation notify states) and **Number Pops** (floating damage/healing numbers displayed via a controller component).

---

## Classes

### Context Effects

| Class | Purpose |
|-------|---------|
| `IContextEffectsInterface` | Blueprintable interface — `AnimMotionEffect()` call from AnimNotify states; implemented by `UContextEffectComponent` |
| `UContextEffectComponent` | `UActorComponent`, implements `IContextEffectsInterface` — holds a set of `UContextEffectsLibrary` assets and a `DefaultEffectContexts` tag container; on `AnimMotionEffect`: resolves physical surface to a context tag, queries libraries, spawns matching sounds and Niagara systems via `UContextEffectsSubsystem` |
| `UContextEffectsLibrary` | `UObject` DataAsset — array of `FContextEffects` entries (`EffectTag` + `Context` tags → soft refs to `USoundBase` / `UNiagaraSystem`); async-loads assets on demand; supports `GetEffects()` with `ExactMatch` / `BestMatch` resolution |
| `UContextEffectsSubsystem` | `UWorldSubsystem` — per-actor library registry; `SpawnContextEffects()`, `GetContextFromSurfaceType()`, `LoadAndAddContextEffectsLibraries()`, `UnloadAndRemoveContextEffectsLibraries()` |
| `UContextEffectsSettings` | `UDeveloperSettings` (`DefaultGame.ini`) — `SurfaceTypeToContextMap` (`EPhysicalSurface → FGameplayTag`); maps physical material surfaces to context tags at project level |

### Number Pops

| Class | Purpose |
|-------|---------|
| `UNumberPopComponent` | **Abstract** `UControllerComponent` — base class for floating number display; `AddNumberPop(FNumberPopRequest)` virtual; subclass in Blueprint and implement the actual widget/mesh presentation |
| `FNumberPopRequest` | Struct — `WorldLocation`, `SourceTags`, `TargetTags`, `NumberToDisplay` (int32), `bIsCriticalDamage` |
| `UDamagePopStyle` | `UDataAsset` — visual style definition per damage type: `DisplayText`, `MatchPattern` (`FGameplayTagQuery`), `Color`, `CriticalColor`, `TextMesh` |

---

## How It Works

### Context Effects Pipeline
1. An AnimNotify state calls `IContextEffectsInterface::AnimMotionEffect()` on the owning actor.
2. `UContextEffectComponent` resolves the physical surface from the `HitResult` to a context tag via `UContextEffectsSettings::SurfaceTypeToContextMap` (if `bConvertPhysicalSurfaceToContext` is enabled).
3. The merged context tag container is passed to `UContextEffectsSubsystem::SpawnContextEffects()`.
4. The subsystem queries the actor's loaded `UContextEffectsLibrary` set for matching `FContextEffects` entries and spawns the resolved sounds and Niagara systems attached to the specified bone.

### Number Pops Pipeline
1. Game code constructs a `FNumberPopRequest` and calls `UNumberPopComponent::AddNumberPop()` on the player controller.
2. The Blueprint subclass handles the actual display (e.g., spawning a widget or instanced mesh at `WorldLocation`).
3. `UDamagePopStyle` assets matched via `MatchPattern` determine the visual style (color, mesh, label).

---

## Placement

| Where | What |
|-------|------|
| **Pawn / Character** | `UContextEffectComponent` |
| **World** | `UContextEffectsSubsystem` (automatic — `UWorldSubsystem`) |
| **PlayerController** | Blueprint subclass of `UNumberPopComponent` |
| **DataAsset** | `UContextEffectsLibrary` (one per pawn type / surface set) |
| **DataAsset** | `UDamagePopStyle` (one per damage category) |
| **Project Settings** | `UContextEffectsSettings` — `SurfaceTypeToContextMap` |

---

## Messages (GameplayMessageSubsystem)

None — `AnimMotionEffect` is called directly via the interface.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `ModularGameplay` | `UControllerComponent` base for `UNumberPopComponent` |
| `GameplayTags` | Context tags, effect tags, tag queries |
| `Niagara` | VFX spawning |
| `AudioMixer` | Sound spawning |
| `PhysicsCore` | `EPhysicalSurface` for surface-to-context mapping |
| `DeveloperSettings` | `UContextEffectsSettings` |
| `Elemental_Interfaces` | Pawn interfaces |
