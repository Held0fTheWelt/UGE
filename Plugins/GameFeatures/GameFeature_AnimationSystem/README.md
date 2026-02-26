# GameFeature_AnimationSystem

Animation base system. Provides `UFeaturedAnimInstance` — a `UAnimInstance` subclass that automatically binds to the owning pawn's `UAbilitySystemComponent`, maps gameplay tags to AnimBP properties, and computes ground distance for movement animations.

---

## Classes

### AnimInstance
| Class | Purpose |
|-------|---------|
| `UFeaturedAnimInstance` | Extends `UAnimInstance` — GAS-aware AnimBP base; binds `FGameplayTagBlueprintPropertyMap`, caches ASC reference, computes `GroundDistance` via `ICharacterGroundInfoProviderInterface` |

---

## How It Works

### Tag-Property Mapping
`FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap` — data-driven binding of gameplay tags to Blueprint properties of the AnimBP. Initialized in `InitializeWithAbilitySystem()`. No C++ code required for new tag-to-property bindings.

### Ground Distance
`NativeUpdateAnimation()` computes `GroundDistance` each frame:
1. Searches pawn components for `ICharacterGroundInfoProviderInterface`
2. Falls back to checking the owner actor directly
3. Sets `GroundDistance = -1.0f` if no provider is found

### GAS Queries (Blueprint API)
| Method | Purpose |
|--------|---------|
| `InitializeWithAbilitySystem(ASC)` | Binds the TagPropertyMap to the ASC |
| `HasGameplayTag(Tag)` | Returns true if the tag is present on the ASC |
| `IsAbilityActive(Tag)` | Checks whether the ability tag is in the owned tags |
| `GetAttributeValue(Tag)` | Returns an attribute value *(stub — returns 0.0f, TODO)* |

---

## Usage

In Blueprint: derive the AnimBP from `UFeaturedAnimInstance` (C++ class).
In Class Defaults: populate `GameplayTagPropertyMap` — map tags to the desired AnimBP float/bool/enum properties.

---

## Placement

| Where | What |
|-------|------|
| **Pawn / Character** | Set the AnimBP class to a subclass of `UFeaturedAnimInstance` |
| **Pawn components or owner** | Implement `ICharacterGroundInfoProviderInterface` (optional, for `GroundDistance`) |

---

## Messages (GameplayMessageSubsystem)

None.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `AnimationCore` | `UAnimInstance` base |
| `GameplayAbilities` | `UAbilitySystemComponent`, `FGameplayTagBlueprintPropertyMap` |
| `GameplayTags` | Tag queries |
| `Elemental_Interfaces` | `ICharacterGroundInfoProviderInterface` |
