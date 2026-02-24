using UnrealBuildTool;
 
public class Elemental_Logging : ModuleRules
{
	public Elemental_Logging(ReadOnlyTargetRules Target) : base(Target)
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
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
			}
            );
    }
}