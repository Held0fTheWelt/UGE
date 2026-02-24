using UnrealBuildTool;
 
public class Elemental_Player : ModuleRules
{
	public Elemental_Player(ReadOnlyTargetRules Target) : base(Target)
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
                "AIModule",
                "Elemental_Classes",
                "Elemental_Structure",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "InputCore",
                "EnhancedInput",
                "GameplayTags",
                "GameplayAbilities",
                "ModularGameplay",
                "Elemental_Classes",
                "Elemental_Common",
                "Elemental_Interfaces",
                "Elemental_Structure",
                "GameplayMessageRuntime",
                "NetCore",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}