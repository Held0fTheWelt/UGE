# UGE TTD Showcase

UGE is the showcase project for Tiny Tool Development plugin output. It is the code and runtime
integration target. Generated content packs stay in `D:/UGE_ContentExamples` and are consumed through
the external plugin root `../UGE_ContentExamples/AssetPlugins`.

## Current Lyra / WarCollection Intake

- Source content: Lyra Starter Game, scanned and compiled by Asset Plugin Maker.
- Reference architecture: `E:/WarCollection2`.
- Code target: `D:/UGE`.
- Content-pack target: `D:/UGE_ContentExamples`.
- Deprecated target: `D:/TinyToolDevelopment/WarCollection`.

The active content-pack plan is pinned by:

```text
plan_id: lyra-5.4-to-uge-contentexamples-694f9c382198
plan_hash: 8461d263d8d00eafda40e7af5016f5d013903a55
asset_count: 1922
content_packs: 10
composition_rebuilds: 1244
```

## Source-Controlled Showcase Contracts

`Config/TTDShowcase/ttd-showcase-manifest.json` is the UGE intake manifest. It declares:

- split target roots;
- enabled generated content packs and mounts;
- expected APM / UMCP routes;
- BPJ target contracts for old Blueprint, World, Data, animation, UI, equipment, and map assemblies.

`Config/TTDShowcase/bpj-composition-rebuild-queue.json` is the full BPJ rebuild queue for the
excluded old compositions. It keeps all 1,244 rebuild items grouped by target architecture.

These files are intentionally generic. A future APM run for another project should be able to emit
the same shape, with UGE using it as the integration contract instead of hardcoded Lyra logic.

## Composition Policy

Old Lyra Blueprint, World, and Data compositions are not copied into UGE as authoritative runtime
structure. APM owns visible asset packaging. BPJ owns the reviewable implementation intent for
excluded compositions. UGE owns the target-side implementation contracts.

Current target groups:

| Target group | Count | UGE owner |
| --- | ---: | --- |
| AnimationComposition | 724 | `GameFeature_AnimationSystem` |
| GameplayComposition | 285 | `Core_Collection` and target GameFeatures |
| UIComposition | 128 | `Core_Classes` UI layer |
| DataComposition | 64 | `Core_Data` and `AssetLoader` |
| ExperienceComposition | 25 | `GameFeature_ExperienceSystem` and `Core_Data` |
| MapAssembly | 7 | target-native maps assembled from content packs |
| EquipmentComposition | 6 | `GameFeature_EquipmentSystem` and `GameFeature_InventorySystem` |
| EditorTooling | 5 | discard by default, unless a UGE editor tool is explicitly planned |

## Verification

Headless mount smoke:

```powershell
& 'D:/Engines/UE_5.6/Engine/Binaries/Win64/UnrealEditor-Cmd.exe' `
  'D:/UGE/UGE.uproject' `
  -unattended -nop4 -NullRHI -NoSound -nosplash `
  -run=ResavePackages -ProjectOnly -SkipSave -NoShaderCompile `
  -Package=/LyraUIAssets/UI/Foundation/Materials/M_UI_Throbber_Base
```

Automation harness:

```powershell
& 'D:/Engines/UE_5.6/Engine/Binaries/Win64/UnrealEditor-Cmd.exe' `
  'D:/UGE/UGE.uproject' `
  -unattended -nop4 -NullRHI -NoSound -nosplash `
  -ExecCmds='Automation RunTests Project.UGE.TTDShowcase; Quit' `
  -TestExit='Automation Test Queue Empty'
```

The full UBT build may wait behind unrelated running Unreal builds because UBT serializes with
`-WaitMutex`. The mount smoke is the quick gate for content-pack availability; the automation harness
is the stronger gate once UBT can compile the test module.
