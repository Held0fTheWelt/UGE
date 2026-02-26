# GameFeature_FeedbackSystem

Feedback system for visual and audio reactions to gameplay events. Two subsystems:
1. **Context Effects** — spawns Niagara VFX and sounds based on surface tags (footsteps, impacts, etc.)
2. **Number Pops** — displays damage and healing numbers as HUD overlays

---

## Classes

### Context Effects
| Class | Purpose |
|-------|---------|
| `UContextEffectComponent` | `UActorComponent`, implements `IContextEffectsInterface` — receives `AnimMotionEffect` callbacks from AnimNotifies; delegates spawning to `UContextEffectsSubsystem` |
| `IContextEffectsInterface` | Interface — defines the `AnimMotionEffect()` callback; implemented by `UContextEffectComponent` |
| `UContextEffectsLibrary` | `UObject` — data library: maps effect tags to `USoundBase` / `UNiagaraSystem` arrays |
| `UActiveContextEffects` | `UObject` — a running effect instance (sound + Niagara) |
| `FContextEffects` | Struct — sound and Niagara system pair |
| `UContextEffectsSubsystem` | `UWorldSubsystem` — central spawning; resolves tags against `UContextEffectsLibrary`; respects surface type mapping |
| `UContextEffectsSettings` | `UDeveloperSettings` — configuration: default libraries, surface-to-tag mapping |

### Number Pops
| Class | Purpose |
|-------|---------|
| `UNumberPopComponent` | **Abstract** `UControllerComponent` — base class for damage popups; subclass in Blueprint and implement `AddNumberPop()` |
| `UDamagePopStyle` | `UDataAsset` — style configuration: colors, font sizes, separators per gameplay tag |

---

## Placement

| Where | What |
|-------|------|
| **Any actor** | `UContextEffectComponent` |
| **World** | `UContextEffectsSubsystem` (automatic) |
| **PlayerController** | Blueprint subclass of `UNumberPopComponent` |
| **DataAsset** | `UContextEffectsLibrary`, `UDamagePopStyle` |

---

## Messages (GameplayMessageSubsystem)

None — `AnimMotionEffect` is called directly via the interface.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `Niagara` | Particle effects |
| `AudioMixer` | Sound spawning |
| `PhysicsCore` | `EPhysicalSurface` for surface type mapping |
| `GameplayTags` | Tag-based effect selection |
| `Elemental_Interfaces` | Pawn interfaces |
| `DeveloperSettings` | `UContextEffectsSettings` |
