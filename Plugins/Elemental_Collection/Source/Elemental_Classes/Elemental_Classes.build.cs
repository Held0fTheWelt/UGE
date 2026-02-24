using UnrealBuildTool;
 
public class Elemental_Classes : ModuleRules
{
	public Elemental_Classes(ReadOnlyTargetRules Target) : base(Target)
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
                "CoreUObject",
                "AssetLoader",
                "Elemental_Common",
                "Elemental_Data",
                "Elemental_Interfaces",
                "Elemental_Structure",
                "ModularGameplay",  // UGameStateComponent, Components/GameStateComponent.h
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
                "GeometryFramework",
                "AIModule",
                "GameplayTags",
                "LoggingMacros",
                "UMG",
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