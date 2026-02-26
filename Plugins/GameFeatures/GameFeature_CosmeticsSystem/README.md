# GameFeature_CosmeticsSystem

Cosmetics system for character parts. Manages modularly swappable character meshes (e.g. head, body, equipment pieces) per pawn with replication. The controller determines which parts are spawned; the pawn spawns and manages the actual actor instances.

---

## Classes

### Components
| Class | Purpose |
|-------|---------|
| `UControllerComponent_CharacterParts` | `UControllerComponent` — holds the list of desired `FCharacterPart` entries for the possessed pawn; delegates the actual spawning to `UPawnComponent_CharacterParts` |
| `UPawnComponent_CharacterParts` | `UActorComponent` — spawns/despawns cosmetic actors, replicates the active part list via `FFastArraySerializer`; exposes `FSpawnedCharacterPartsChanged` delegate |

### Types
| Struct/Enum | Purpose |
|-------------|---------|
| `FCharacterPart` | A single cosmetic part: actor class + optional socket name |
| `FCharacterPartHandle` | Opaque handle identifying a spawned part |
| `ECharacterCustomizationCollisionMode` | Collision mode for spawned parts (NoCollision, UseProfileFromPart) |

### Animation
| Struct | Purpose |
|--------|---------|
| `FCosmeticAnimLayerSelectionEntry` | Maps a gameplay tag to an anim layer type |
| `FCosmeticAnimLayerSelectionSet` | Lists multiple `FCosmeticAnimLayerSelectionEntry` entries; selects the first match |
| `FCosmeticAnimBodyStyleSelectionEntry` | Maps body style tags to an AnimBP type |
| `FCosmeticAnimBodyStyleSelectionSet` | Lists multiple body style selectors |

---

## Placement

| Where | What |
|-------|------|
| **PlayerController** | `UControllerComponent_CharacterParts` |
| **Pawn** | `UPawnComponent_CharacterParts` |

---

## Messages (GameplayMessageSubsystem)

None — communication via the `FSpawnedCharacterPartsChanged` delegate on `UPawnComponent_CharacterParts`.

---

## Dependencies

| Module | Usage |
|--------|-------|
| `Engine` | `UActorComponent`, `AActor` |
| `ModularGameplay` | `UControllerComponent` |
| `GameplayTags` | Tag-based anim layer / body style selection |
| `Elemental_Interfaces` | Pawn interfaces |
| `NetCore` | Replication helpers |
