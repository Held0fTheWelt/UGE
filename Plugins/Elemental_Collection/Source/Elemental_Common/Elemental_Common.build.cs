using UnrealBuildTool;
 
public class Elemental_Common : ModuleRules
{
	public Elemental_Common(ReadOnlyTargetRules Target) : base(Target)
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
                "InputCore",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
                "CommonInput",
                "CommonUI",
                "CommonUser",
                "GameplayTags",
                "Elemental_Interfaces",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}