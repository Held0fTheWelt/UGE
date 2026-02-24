using UnrealBuildTool;
 
public class Core_Tests : ModuleRules
{
    public Core_Tests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
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
                "Core_AbilitySystem",
                "Core_Classes",
                "Elemental_Classes",
                "Elemental_Interfaces",
                "ExternalRpcRegistry",
                "Gauntlet",
                "GameplayAbilities",
                "GameplayTags",
                "HTTPServer",
                "Json",
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
