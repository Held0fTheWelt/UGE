using UnrealBuildTool;
 
public class Core_Interfaces : ModuleRules
{
	public Core_Interfaces(ReadOnlyTargetRules Target) : base(Target)
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
                "Engine",               // UWorldSubsystem, AController
                "InputCore",            // FKey (KeyboardInputSettingInterface)
                "AssetRegistry",        // FAssetData, FPrimaryAssetId
                "GameplayTags",          // FGameplayTagQuery (FCorePerformanceStatGroup)
                "GameplayAbilities",     // UAbilitySystemComponent (PawnDataAbilitySetApplier)
                "Elemental_Interfaces",
                "Elemental_Structure",
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