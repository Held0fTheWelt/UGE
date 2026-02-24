using UnrealBuildTool;
 
public class Core_Data : ModuleRules
{
	public Core_Data(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Core_Structure",
                "Core_Interfaces",            // PerformanceStatTypes (UCoreSettingsLocal, Plan Paket 4)
                "GameSubtitles",              // SubtitleDisplayOptions (UCoreSettingsShared)
                "Elemental_Interfaces",       // ISharedSettingsForceFeedbackInterface, IAudioSystemInterface
				"GameFeatures",
                "CommonUser",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "Slate",
                "SlateCore",
                "LoggingMacros",
                "AssetLoader",
                "Core_Interfaces",
                "Elemental_Data",
                "Elemental_Structure",
                "Elemental_Common",           // UElementalAudioProviderSubsystem (ApplyVolumeToAudioBuses)
                "GameSettings",               // ULocalPlayerSaveGame (UCoreSettingsShared)
                "InputCore",                  // InputCoreTypes (UCoreSettingsLocal)
                "AudioModulation",            // SoundControlBus, SoundControlBusMix
                "AudioMixer",                 // (if needed by AudioModulation)
                "EnhancedInput",              // EnhancedInputUserSettings (CoreSettingsShared Save)
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}