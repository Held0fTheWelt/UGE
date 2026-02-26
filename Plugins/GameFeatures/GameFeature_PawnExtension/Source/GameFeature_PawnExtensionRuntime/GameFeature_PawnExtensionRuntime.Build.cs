using UnrealBuildTool;

public class GameFeature_PawnExtensionRuntime : ModuleRules
{
	public GameFeature_PawnExtensionRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"ModularGameplay",
				"GameplayAbilities",
				"GameplayTags",
			}
			);
			
		
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Elemental_Classes",
                "Elemental_Interfaces",
                "Elemental_Structure",
                "Core_Structure",		// GameplayTags_Core (InitState_*, Ability_Behavior_SurvivesDeath)
                "NetCore",
				// ... add private dependencies that you statically link with here ...	
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
