using UnrealBuildTool;
 
public class Core_HotfixSystem : ModuleRules
{
	public Core_HotfixSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "OnlineSubsystem",
                "OnlineHotfix",
                "DeveloperSettings",
                "Core_Classes",
                "Core_Data",              // UCoreSettingsLocal
                "Core_Setup",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
