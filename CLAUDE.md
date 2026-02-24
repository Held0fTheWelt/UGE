# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**UGE (Unified Game Equipment)** is an Unreal Engine 5.6 project built on top of Epic's Lyra starter game framework. It targets Windows and supports multiple online backend configurations (Steam, Epic Online Services).

## Build Commands

All UE5 build commands require the engine's UAT/UBT tools. The project uses `RunUAT.bat` from the engine installation. Batch scripts are in `Build/BatchFiles/`.

### Package a local build (Development, Win64)
```bat
Build\BatchFiles\RunLocalPackage.bat
```

### Package with EOS support
```bat
Build\BatchFiles\RunLocalPackage_EOS.bat
```

### Run automated tests via BuildGraph
```bat
Build\BatchFiles\RunLocalTests.bat
```

### Run localization
```bat
Build\BatchFiles\RunLocalize.bat
```

### Direct UBT compilation (from engine root)
```
Engine\Build\BatchFiles\Build.bat UGEEditor Win64 Development -Project="<path>\UGE.uproject"
```

## Build Targets

The project has multiple target configurations in `Source/`:

| Target File | Type | Description |
|---|---|---|
| `UGEGame.Target.cs` | Game | Base game target; defines `ApplySharedTargetSettings()` used by all targets |
| `UGEEditor.Target.cs` | Editor | Editor build; enables RemoteSession plugin |
| `UGEClient.Target.cs` | Client | Client-only target |
| `UGEServer.Target.cs` | Server | Dedicated server; enables checks in Shipping |
| `UGEGameEOS.Target.cs` | Game | Game with Epic Online Services |
| `UGEGameSteam.Target.cs` | Game | Game with Steam |
| `UGEGameSteamEOS.Target.cs` | Game | Game with both Steam and EOS |
| `UGEServerSteam.Target.cs` | Server | Server with Steam |
| `UGEServerSteamEOS.Target.cs` | Server | Server with Steam and EOS |

`UGEGameTarget.ApplySharedTargetSettings()` centralizes shared configuration (shadow variable warnings as errors, shipping security hardening, disabling OpenImageDenoise for non-editor builds, etc.) and is called by all other targets.

## Plugin Directory Structure

The `.uproject` registers five custom plugin directories beyond the engine defaults:

- `CorePlugins/` — Core framework plugins (CommonLoadingScreen, CommonUser, GameSettings, GameSubtitles, GameplayMessageRouter, PocketWorlds)
- `AssetPlugins/` — Asset-specific plugins
- `EditorPlugins/` — Editor-only plugins
- `WidgetPlugins/` — UI/widget plugins
- `SamplePlugins/` — Sample/demo plugins

Game Feature Plugins are expected under `Plugins/GameFeatures/` and are configured dynamically in `UGEGameTarget.ConfigureGameFeaturePlugins()`. Key rules for Game Feature Plugins:
- Must set `"EnabledByDefault": false`
- Must set `"ExplicitlyLoaded": true`
- Can be suppressed with `"NeverBuild": true` or `"RestrictToBranch": "<name>"`

## Main Game Module

`Source/UGE/` — the primary runtime module. Dependencies: Core, CoreUObject, Engine, InputCore, EnhancedInput.

## CI / BuildGraph

- `Build/LyraBuild.xml` — Compiles tools, editor, and game targets; optionally submits PCBs to Perforce for UnrealGameSync
- `Build/LyraTests.xml` — Full build-and-test pipeline including content validation, cook tests, localization, audit collection updates, Epic Game Store deployment, and PGO profiling/optimization

## Branch Strategy

- `master` — Stable, production-ready
- `EMPTY_BASE` — Clean base for creating new feature branches
- `Build` — Latest build configuration (current branch)

## Key Conventions

- Engine version: **UE 5.6** (`IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6`)
- Build settings: `BuildSettingsVersion.V5`
- Shadow variable warnings are treated as **errors** in Unique build environments
- Shipping builds disable unverified HTTPS certificates and restrict generated/non-UFS ini files
- The `IsBuildMachine` environment variable (`== "1"`) controls build machine–specific behavior
