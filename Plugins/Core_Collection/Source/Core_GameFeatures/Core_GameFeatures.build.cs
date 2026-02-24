using UnrealBuildTool;
 
public class Core_GameFeatures : ModuleRules
{
	public Core_GameFeatures(ReadOnlyTargetRules Target) : base(Target)
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
                "GameplayTags",
                "EnhancedInput",
                "ModularGameplay",
                "GameFeatures",
                "GameplayAbilities",
                "UMG",
                "Elemental_Interfaces",   // GameFeatureEvents::NAME_AbilityReady for extension event
                "Elemental_Network",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}