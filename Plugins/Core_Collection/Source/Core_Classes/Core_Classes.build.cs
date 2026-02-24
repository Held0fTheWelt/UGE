using UnrealBuildTool;
 
public class Core_Classes : ModuleRules
{
	public Core_Classes(ReadOnlyTargetRules Target) : base(Target)
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
                "Core_Data",                 // UCoreSettingsLocal, UCoreSettingsShared (Settings types)
                "Core_Interfaces",
                "Core_Logging",
                "LoggingMacros",
                "AssetLoader",
                "Elemental_Classes",
                "Elemental_Data",
                "Elemental_Interfaces",
                "Elemental_Logging",
                "Elemental_Structure",
                "InputCore",       // FKey, EKeys
                "AIModule",       // IGenericTeamAgentInterface, FGenericTeamId
                "NetCore",        // UEPushModelPrivate::MarkPropertyDirty
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Elemental_Classes",
                "CoreUObject",
                "Engine",
                "ApplicationCore",   // IPlatformInputDeviceMapper, platform APIs
                "Slate",
                "SlateCore",
                "ModularGameplay",
                "ChaosVehicles",
                "PhysicsCore",
                "UMG",
                "CommonUI",                 // For CommonActivatableWidget, CommonUserWidget
                "CommonInput",              // UCommonInputSubsystem, UCommonInputSettings, UCommonInputPlatformSettings
                "EnhancedInput",            // UInputAction, UInputMappingContext, UEnhancedInputLocalPlayerSubsystem
                "EngineSettings",           // Common platform settings
                "DeveloperSettings",         // UPlatformSettingsManager, GetSettingsForPlatform
                "GameplayAbilities",       // For AbilitySystemComponent in HUD
                "GameplayTasks",            // For AbilitySystemComponent
                "GameplayTags",             // For GameplayTags
                "Core_Structure",           // For GameplayTags_Core (tag definitions)
                "GameplayMessageRuntime",   // For GameplayMessageSubsystem (plugin: CorePlugins/GameplayMessageRouter)
                "Elemental_Player",         // For ITeamAgentInterface
                "Elemental_Common",         // For UCommonLocalPlayer
                "CommonUser",               // For CommonUserSubsystem
                "AudioMixer",               // For AudioMixerBlueprintLibrary
                "GameSettings",             // For IGameSettingActionInterface (CoreBrightnessEditor, CoreSafeZoneEditor)
                "CommonUser",              // For CommonUserSubsystem
                "AudioMixer",              // For AudioMixerBlueprintLibrary
                "GameSettings",            // For IGameSettingActionInterface (CoreBrightnessEditor, CoreSafeZoneEditor)
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