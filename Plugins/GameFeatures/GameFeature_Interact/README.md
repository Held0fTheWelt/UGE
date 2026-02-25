# GameFeature_Interact

Unreal Engine **5.6** — Game Feature Plugin im UGE-Framework.

Stellt ein kollisionsbasiertes Interaktionssystem bereit: Akteure können Interaktionszonen definieren, die bei Annäherung eines Pawns ein UI-Widget einblenden, den Controller registrieren, Sense-Systeme informieren und auf den Interact-Input reagieren.

---

## Plugin-Konfiguration

| Eigenschaft | Wert |
|-------------|------|
| Module | `GameFeature_InteractRuntime` (Runtime, Default) · `GameFeature_InteractSetup` (Runtime, Default) |
| Initial State | **Active** (`BuiltInInitialFeatureState: Active`) |
| Explizit geladen | `true` |
| Sealed | `false` |

**Plugin-Abhängigkeiten:** `AssetLoader` · `Elemental_Collection` · `Core_Collection` · `Warfare_Assets` · `Vehicles_Test_Assets`

---

## Einordnung im UGE-Layer-Modell

```
┌────────────────────────────────────────────────────────────────┐
│  Game Features      GameFeature_Interact           ← hier      │
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

## Verzeichnisstruktur

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
    │       │   ├── InteractCollisionComponent.cpp         — Konstruktor, BeginPlay, Lifecycle, Limit
    │       │   ├── InteractCollisionComponent_Setup.cpp   — Asset-Loading, Box-Erstellung, ShapeUpdated
    │       │   ├── InteractCollisionComponent_Interact.cpp — Overlap-Events, Sense, Controller-Registrierung
    │       │   └── InteractCollisionComponent_Widget.cpp  — Widget anzeigen/entfernen
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

## Kernklassen

### `UInteractCollisionComponent`

**Vererbung:**
```
UActorComponent
  └─ UCombinedBundleActorComponent   — Asset-Loading-Framework, GameFeatureName-Tracking
       └─ UPawnDrivingActorComponent_Base  — PawnOwner / PawnController, Possession-Events
            └─ UInteractCollisionComponent
                 implements IInteractControlInterface   — Interact() / StopInteracting()
                 implements IShapeUpdatedInterface      — ShapeUpdated()
```

**Initialisierungswerte (Konstruktor):**

| Eigenschaft | Wert | Bedeutung |
|-------------|------|-----------|
| `GameFeatureName` | `"Interact"` | Verknüpft Asset-Loading mit dieser Feature |
| `bComponentHasComponentAssetToLoad` | `true` | Lädt eigene Komponenten-Assets (nicht Actor-Level) |
| `bHasLimit` | `false` | Kein Limit by default |
| `Limit` | `1` | Standardwert falls Limit aktiviert wird |
| `PrimaryComponentTick.bCanEverTick` | `true` | Tick aktiv (Editor-only für Debug-Zeichnung) |

**Transiente Laufzeit-Properties:**

| Property | Typ | Bedeutung |
|----------|-----|-----------|
| `InteractableComponents` | `TArray<UBoxComponent*>` | Erzeugte Kollisionsboxen |
| `InteractWidgetInstance` | `UUserWidget*` | Aktuell angezeigte UI-Instanz |
| `InteractUIImage` | `TSoftObjectPtr<UTexture2D>` | Interaktions-Icon |
| `WidgetTemplate` | `TSoftClassPtr<UUserWidget>` | Geladene Widget-Klasse |
| `InteractableActors` | `TArray<AActor*>` | Alle aktuell überlappenden Akteure |
| `CurrentInteractingActor` | `AActor*` | Zuletzt interagierender Aktor |
| `bHasLimit` | `bool` | Ob ein Interaktionslimit gilt |
| `Limit` | `int32` | Verbleibende Interaktionen |
| `TurnedCollisionOnElement` | `TArray<bool>` | 90°-Rotation je Kollisionsbox |

**Editor-only Properties:**

| Property | Standard | Bedeutung |
|----------|----------|-----------|
| `bDebugMode` | `true` | Zeichnet grüne Debug-Boxen um alle Kollisionskomponenten (TickComponent) |
| `bDebugCollisionBoxComponent` | `false` | Macht Box-Komponenten im Spiel sichtbar |

---

### `UGameFeature_Interact_Settings`

Globale Konfiguration. Gespeichert in `DefaultGame.ini` (`Config = Game, defaultconfig`).

| Property | EditCondition | Bedeutung |
|----------|--------------|-----------|
| `bUseGlobalQueryCollisionDebug` | — | Master-Switch für Kollisions-Debug |
| `bGlobalQueryCollisionDebug` | `bUseGlobalQueryCollisionDebug` | Aktiviert globales Kollisions-Debug |
| `DefinedCollisionDebug` | — | Akteur-Klassen mit Debug-Visualisierung (`HideInDetailPanel`) |
| `bUseGlobalDefaultInteractLimit` | — | Master-Switch für globales Interaktionslimit |
| `bHasDefaultInteractLimit` | `bUseGlobalDefaultInteractLimit` | Ob das globale Limit aktiv ist |
| `DefaultInteractLimit` | `bUseGlobalDefaultInteractLimit` | Globaler Limit-Standardwert |

---

### `UInteractUserWidgetBase`

Base-Widget-Klasse für die Interaktionsanzeige.

**Vererbung:** `UUserWidget` + `IInteractWidgetInterface`

| Element | Typ | Binding |
|---------|-----|---------|
| `Text` | `UTextBlock*` | `BindWidget` — Interaktionstext |
| `Image` | `UImage*` | `BindWidget` — Interaktions-Icon |

Implementiert `IInteractWidgetInterface`:
- `SetImage_Implementation(UTexture2D*)` — setzt das Icon-Bild
- `SetText_Implementation(const FString&)` — setzt den Beschriftungstext

Flags: `DisableNativeTick`, `editinlinenew`, `Blueprintable`

---

### `IInteractWidgetInterface`

Schnittstelle zwischen `UInteractCollisionComponent` und dem Widget.

| Methode | Ereignis |
|---------|---------|
| `SetImage(UTexture2D*)` | Icon setzen (`BlueprintNativeEvent`) |
| `SetText(const FString&)` | Text setzen (`BlueprintNativeEvent`) |

---

## Asset-Loading-Typen

`SetupLoadedAsset()` unterscheidet drei Primary-Asset-Typen und reagiert entsprechend:

| Asset-Typ | Klasse | Effekt |
|-----------|--------|--------|
| `"WidgetDefinition"` | `UWidgetDefinitionDataAsset` | Widget-Klasse → `WidgetTemplate` |
| `"CollisionBoxConfig"` | `UCollisionBoxConfigDataAsset` | Rotationsflag → `TurnedCollisionOnElement`, danach `BuildInteractableCollisionComponents()` |
| `"ActorUIImage"` | `UActorUIImageDataAsset` | Textur → `InteractUIImage` |

---

## Ablauf

### Initialisierung

```
[Assets geladen durch UCombinedBundleActorComponent]
SetupLoadedAsset() — wiederholt aufgerufen, einmal pro Asset:

  PrimaryAssetType == "WidgetDefinition"
    → WidgetClass synchron laden
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
    → Image synchron laden
    → InteractUIImage = TSoftObjectPtr(Texture)
```

### Laufzeit — Overlap-Events

```
OnBoxComponentBeginOverlap(OtherActor)
  ├─ OtherActor == Owner  →  return
  ├─ InstigatorController == null  →  return
  ├─ !CheckLimit()  →  return (Limit erreicht)
  ├─ InstigatorController == FirstPlayerController
  │    └─ ShowWidget()
  │         ├─ WidgetTemplate.LoadSynchronous() (falls nötig)
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

### Interact-Input

```
Interact_Implementation(InstigatorController)
  ├─ Owner muss IInteractInterface implementieren
  └─ IInteractInterface::GetOnInteractWithPawn()
       .Broadcast(InstigatorController->GetPawn(), InstigatorController)
```

Das Broadcast-Signal empfangen `ACombinedBundleActor/Pawn/Character` und `CombinedWheeledVehiclePawn` über ihre `FOnInteractWithPawn`-Delegate-Bindung und delegieren an den `InteractManageGameInstanceSubsystem` für Pawn-Swapping.

---

## Interaktionslimit

`CheckLimit()` gibt `true` zurück wenn:
- `bHasLimit == false` (kein Limit gesetzt), **oder**
- `bHasLimit == true` und `Limit > 0`

Das globale Standardlimit kann über `UGameFeature_Interact_Settings` gesetzt werden.

---

## Lifecycle-Events

| Event | Verhalten |
|-------|-----------|
| `BeginPlay` | Prüft `IsGameFeatureIgnored()`, loggt Start |
| `ProcessPossess` | Entfernt Widget wenn der erste Spieler den Besitzer possessiert |
| `EndPlay` | Entfernt Widget unabhängig vom Grund |
| `ShapeUpdated_Implementation` | Aktualisiert `InteractableComponents[0]` mit neuen Mesh-Bounds |

---

## Anforderungen an den besitzenden Aktor

| Interface | Slot | Zweck |
|-----------|------|-------|
| `ISceneComponentCatchInterface` | `"Mesh"` | Liefert die Mesh-Komponente für Bound-Berechnung der Kollisionsbox |
| `IInteractInterface` | — | Empfängt `FOnInteractWithPawn`-Broadcasts |

Der Controller des interagierenden Pawns **sollte** `IRegisterComponentInterface` implementieren, damit sich der Component beim Controller an-/abmelden kann.

---

## CoreInformation-Editor-Integration

`GetSettingsVariableSets_Implementation()` liefert drei `FInformationSet`-Gruppen:

| Gruppe | Inhalt |
|--------|--------|
| `"Interact"` | `HasLimit` (bool), `Limit` (int, nur wenn `bHasLimit == true`) |
| `"Current Interacting Actor"` | Name des `CurrentInteractingActor` oder `"No actor is currently interacting"` |
| `"Interactable Actors"` | Namen aller Akteure in `InteractableActors` oder `"No actors found"` |

---

## Logging

| Kategorie | Inhalt |
|-----------|--------|
| `Log_Interact` | Allgemeiner Lebenszyklus, Overlap-Events, Limit-Prüfung |
| `Log_Interact_Debug` | Editor-Tick — Sichtbarkeit der Debug-Kollisionsboxen |
| `Log_Interact_Setup` | Asset-Loading, Box-Erstellung, ShapeUpdated |
| `Log_Interact_Widget` | Widget erstellen, anzeigen, entfernen |
| `Log_Interact_Class` | Klassen-spezifische Ereignisse |

Alle Logs verwenden `UE_LOGFMT` mit den Feldern `Time`, `Line`, `Class`.

---

## Modul-Abhängigkeiten

### `GameFeature_InteractRuntime`

| Modul | Sichtbarkeit | Grund |
|-------|-------------|-------|
| `Core` | Public | UE-Basistypen |
| `GameFeature_InteractSetup` | Public | Settings-Klasse im Runtime-Modul nutzbar |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard-UE-Runtime |
| `UMG` | Private | Widget-System (`UUserWidget`, `AddToViewport`) |
| `AssetLoader` | Private | Asset-Manager-Integration |
| `Core_Classes` | Private | `UPawnDrivingActorComponent_Base` |
| `Elemental_Classes` | Private | Basis-Aktor-/Pawn-Klassen |
| `Elemental_Data` | Private | Datentypen aus Elemental_Collection |
| `Elemental_Interfaces` | Private | Alle genutzten Interfaces |
| `Elemental_Logging` | Private | `GlobalLog`, `StructuredLog` |
| `Elemental_Structure` | Private | Shared Structs (`FInformationSet`) |
| `Core_Interfaces` | Private | `IRegisterComponentInterface` u. a. |

### `GameFeature_InteractSetup`

| Modul | Sichtbarkeit | Grund |
|-------|-------------|-------|
| `Core` | Public | UE-Basistypen |
| `CoreUObject` · `Engine` · `Slate` · `SlateCore` | Private | Standard-UE-Runtime |
