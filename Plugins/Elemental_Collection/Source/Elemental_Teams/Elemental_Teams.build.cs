using UnrealBuildTool;
 
public class Elemental_Teams : ModuleRules
{
	public Elemental_Teams(ReadOnlyTargetRules Target) : base(Target)
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
                "NetCore",
                "AIModule",
                "GameplayTags",
                "Elemental_Interfaces",
                "Elemental_Structure",
                "ModularGameplay",
                "Niagara",
             //   "Core_AbilitySystem",
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "GameplayAbilities",        
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