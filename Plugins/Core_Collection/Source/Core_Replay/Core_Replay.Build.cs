using UnrealBuildTool;
 
public class Core_Replay : ModuleRules
{
	public Core_Replay(ReadOnlyTargetRules Target) : base(Target)
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
                "GameplayTags",
                "CommonUI",
                "Core_Classes",
                "Core_Data",              // UCoreSettingsLocal
                "Core_Interfaces",        // CoreGameInstanceDelegates (replay support registration)
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
