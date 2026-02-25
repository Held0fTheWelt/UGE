# GameFeature_PhysicalAwareness

Unreal Engine **5.6** — Game Feature Plugin im UGE-Framework.

Stellt ein kollisionsbasiertes Touch-Sense-System bereit, das einem Pawn erlaubt, physisch überlappende Akteure zu erkennen, zu verwalten und abzufragen.

---

## Plugin-Konfiguration

| Eigenschaft | Wert |
|-------------|------|
| Modul | `GameFeature_PhysicalAwarenessRuntime` (Runtime, Default) |
| Initial State | **Active** (`BuiltInInitialFeatureState: Active`) |
| Explizit geladen | `true` |
| Sealed | `true` |

**Plugin-Abhängigkeiten:** `AssetLoader` · `Elemental_Collection` · `Core_Collection`

---

## Einordnung im UGE-Layer-Modell

```
┌─────────────────────────────────────────────────────────────┐
│  Game Features      GameFeature_PhysicalAwareness  ← hier   │
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

## Verzeichnisstruktur

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
        │   ├── PhysicalAwarenessSenseComponent.cpp        — Konstruktor, BeginPlay, Interface-Implementierungen
        │   ├── PhysicalAwarenessSenseComponent_Sense.cpp  — Overlap-Events, Aktor-Filterung
        │   └── PhysicalAwarenessSenseComponent_Setup.cpp  — Asset-Loading, Kollisionskomponente
        ├── DataAssets/
        │   └── PhysicalAwarenessSenseDataAsset.cpp
        ├── Logging/
        │   └── Logging_GF_PhysicalAwarenessSense.cpp
        └── GameFeature_PhysicalAwarenessRuntimeModule.cpp
```

---

## Kernklassen

### `UPhysicalAwarenessSenseComponent`

**Vererbung:**
```
UActorComponent
  └─ UCombinedBundleActorComponent   — Asset-Loading-Framework, GameFeatureName-Tracking
       └─ UPawnDrivingActorComponent_Base  — PawnOwner / PawnController, Possession-Events
            └─ UPhysicalAwarenessSenseComponent
                 implements IPhysicalSenseInterface
```

**Initialisierungswerte (Konstruktor):**

| Eigenschaft | Wert | Bedeutung |
|-------------|------|-----------|
| `GameFeatureName` | `"PhysicalAwareness"` | Verknüpft Asset-Loading mit dieser Feature |
| `bDoNotRegisterOnPossessControl` | `true` | Registriert sich nicht automatisch beim Controller |
| `bComponentHasComponentAssetToLoad` | `false` | Nutzt das Asset des besitzenden Akteurs, nicht ein eigenes |

**Transiente Laufzeit-Properties:**

| Property | Typ | Bedeutung |
|----------|-----|-----------|
| `bDoesTrackActorsForGameManagement` | `bool` | Master-Switch — deaktiviert die gesamte Kollisionserkennung wenn `false` |
| `bCanSenseActors` | `bool` | Steuert, ob Sense-Abfragen Ergebnisse liefern |
| `bCanTouchActors` | `bool` | Steuert, ob Touch-Interaktionen erlaubt sind |
| `TouchableActors` | `TArray<TSoftObjectPtr<AActor>>` | Alle aktuell überlappenden, registrierten Akteure |
| `CurrentTouchedActor` | `AActor*` | Explizit gesetzter „fokussierter" Aktor (extern verwaltbar) |

---

### `UPhysicalAwarenessSenseDataAsset`

Konfigurationsdaten-Asset. Wird über den `AssetLoader` geladen und von `SetupLoadedAsset()` ausgewertet.

**Vererbung:** `UPreLoadingDataAsset` (Primary Asset Type: `"PhysicalAwareness"`)

**Editierbare Properties** (Kategorie `"Physical Awareness"`):

| Property | Standard | Bedeutung |
|----------|----------|-----------|
| `bDoesTrackActorsForGameManagement` | `false` | Aktiviert Kollisions-Tracking; bei `false` wird die Kollisionskomponente deaktiviert und `TouchableActors` geleert |
| `bCanSenseActors` | `false` | Erlaubt generelles Sensing |
| `bCanTouchActors` | `false` | Erlaubt Touch-Interaktionen |

---

## Ablauf

### Initialisierung

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

[Asset geladen durch UCombinedBundleActorComponent]
SetupLoadedAsset()
  ├─ Prüft PrimaryAssetType == "PhysicalAwareness"
  ├─ Cast → UPhysicalAwarenessSenseDataAsset
  ├─ Übernimmt bDoesTrackActorsForGameManagement / bCanSenseActors / bCanTouchActors
  └─ !bDoesTrackActorsForGameManagement?
       └─ SetCollisionComponentActive(false) + TouchableActors.Empty()
```

### Laufzeit — Overlap-Filterung

```
OnBeginOverlap(OtherActor, OtherComp)
  ├─ !bDoesTrackActorsForGameManagement  →  return
  ├─ OtherComp implements INoTouchCollisionInterface  →  return
  └─ SenseTouchActor(OtherActor, true)
       ├─ Actor hat keine Komponente mit IInteractControlInterface  →  return
       └─ RegisterObject(Actor, ST_TOUCH)
            └─ TouchableActors.AddUnique(Actor)

OnEndOverlap(OtherActor, OtherComp)
  ├─ !bDoesTrackActorsForGameManagement  →  return
  ├─ OtherComp implements INoTouchCollisionInterface  →  return
  └─ SenseTouchActor(OtherActor, false)
       └─ ForgetObject(Actor, ST_TOUCH)
            └─ TouchableActors.Remove(Actor)
```

**Filterregeln zusammengefasst:**

| Bedingung | Ergebnis |
|-----------|----------|
| `bDoesTrackActorsForGameManagement == false` | Overlap komplett ignoriert |
| `OtherComp` implementiert `INoTouchCollisionInterface` | Overlap ignoriert |
| `OtherActor` hat keine Komponente mit `IInteractControlInterface` | Aktor nicht registriert |
| Alle Bedingungen erfüllt | Aktor in `TouchableActors` eingetragen / entfernt |

---

## Interface-Kontrakt (`IPhysicalSenseInterface`)

Alle Methoden sind `private` implementiert und nur über das Interface aufrufbar.

| Methode | Rückgabe | Verhalten |
|---------|----------|-----------|
| `CanTouchActor(Actor)` | `bool` | `true` wenn `Actor` in `TouchableActors` enthalten |
| `CanTouchCurrentActor()` | `bool` | `true` wenn `TouchableActors.Num() > 0` |
| `GetCurrentTouchActor()` | `AActor*` | `TouchableActors[0]` oder `nullptr` |
| `IsTouchingCurrentActor()` | `bool` | `true` wenn `CurrentTouchedActor != nullptr` |
| `SetTouchingCurrentActor(Actor)` | `void` | Setzt `CurrentTouchedActor`; `nullptr` zulässig |
| `GetTouchableActors()` | `TArray<TSoftObjectPtr<AActor>>` | Gibt `TouchableActors` zurück |

> `GetCurrentTouchActor()` und `CanTouchCurrentActor()` basieren auf dem **ersten Element** in `TouchableActors`, nicht auf `CurrentTouchedActor`.
> `CurrentTouchedActor` ist ein separater, extern gesetzter Fokus-Aktor.

---

## Anforderungen an den besitzenden Aktor

Der Aktor, dem `UPhysicalAwarenessSenseComponent` hinzugefügt wird, **muss** folgendes implementieren:

| Interface | Zweck |
|-----------|-------|
| `ISceneComponentCatchInterface` | Liefert die Kollisionskomponente über den Slot-Namen `"Collision"` |

Der Slot-Name `"Collision"` muss eine `UPrimitiveComponent` zurückgeben — andernfalls bleibt der Sense-Component inaktiv und loggt einen Fehler.

---

## CoreInformation-Editor-Integration

`GetSettingsVariableSets_Implementation()` liefert eine `FInformationSet`-Gruppe mit der Kategorie `"Sense Information"` und den drei Werten `DoesTrackActors`, `CanSenseActors`, `CanTouchActors` zur Anzeige im CoreInformation-Editor-Panel.

---

## Logging

| Kategorie | Inhalt |
|-----------|--------|
| `Log_PhysicalSenseComponent` | Allgemeiner Lebenszyklus (BeginPlay, Interface-Aufrufe) |
| `Log_PhysicalSenseComponent_Setup` | Asset-Loading, Kollisionskomponenten-Binding, `SetupLoadedAsset` |
| `Log_PhysicalSenseComponent_Runtime` | Overlap-Events, Registrierung / Vergessen von Akteure |

Alle Logs verwenden `UE_LOGFMT` mit den Feldern `Time`, `Line`, `Class` sowie kontextspezifischen Feldern (z. B. `Actor`, `Result`).

---

## Modul-Abhängigkeiten

| Modul | Sichtbarkeit | Grund |
|-------|-------------|-------|
| `Core` | Public | UE-Basistypen |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard-UE-Runtime |
| `AssetLoader` | Private | `UPreLoadingDataAsset`, Asset-Manager-Integration |
| `Elemental_Data` | Private | Datentypen aus Elemental_Collection |
| `Elemental_Interfaces` | Private | `IPhysicalSenseInterface`, `IInteractControlInterface`, `INoTouchCollisionInterface`, `ISceneComponentCatchInterface` |
| `Elemental_Logging` | Private | `GlobalLog`, `StructuredLog` (UE_LOGFMT-Makros) |
| `Core_Classes` | Private | `UPawnDrivingActorComponent_Base`, `UCombinedBundleActorComponent` |
