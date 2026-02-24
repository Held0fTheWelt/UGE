using UnrealBuildTool;
 
public class Elemental_Structure : ModuleRules
{
	public Elemental_Structure(ReadOnlyTargetRules Target) : base(Target)
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
                "GameplayTags",
                "NetCore",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
                "PhysicsCore",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}