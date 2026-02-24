using UnrealBuildTool;
 
public class Core_Setup : ModuleRules
{
	public Core_Setup(ReadOnlyTargetRules Target) : base(Target)
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
                "Core_Data",                 // UCoreSettingsLocal, UCoreSettingsShared (types; Apply/Save/Load logic stays here)
                "Core_Interfaces",           // ICoreSettingsLocalPlayerInterface, CoreGameInstanceDelegates, PerformanceStat provider
                "GameSubtitles",
                "Elemental_Common",
                "Elemental_Interfaces",
                "GameplayTags",              // FGameplayTagQuery (CoreSettingValueDiscrete_PerfStat)
                "CommonUI",                  // UCommonUIVisibilitySubsystem (FGameSettingEditCondition_PerfStatAllowed)
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameSettings",              // For GameSettingRegistry, FGameSettingDataSourceDynamic
                "PropertyPath",               // FCachedPropertyPath
                "EnhancedInput",             // For Enhanced Input Settings
                "AudioModulation",           // For Audio Control Bus
                "AudioMixer",                // UAudioMixerBlueprintLibrary, UAudioDeviceNotificationSubsystem, FAudioOutputDeviceInfo
                "RHI",                       // GDynamicRHI (CoreSettingValueDiscrete_Resolution)
                "InputCore",                 // For Input Types
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