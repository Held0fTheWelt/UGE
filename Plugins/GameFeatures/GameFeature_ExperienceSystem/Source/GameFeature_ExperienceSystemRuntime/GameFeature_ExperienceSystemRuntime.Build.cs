using UnrealBuildTool;

public class GameFeature_ExperienceSystemRuntime : ModuleRules
{
	public GameFeature_ExperienceSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange(new string[] { });

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GameplayTags",
				"GameFeatures",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CommonLoadingScreen",
				"Elemental_Interfaces",
				"ModularGameplay",
				"CommonUser",
				"Core_Data",
			}
			);

		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}
