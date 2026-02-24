using UnrealBuildTool;
 
public class Core_Performance : ModuleRules
{
	public Core_Performance(ReadOnlyTargetRules Target) : base(Target)
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
                "Core_Interfaces",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "DeveloperSettings",
                "GameplayTags",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
