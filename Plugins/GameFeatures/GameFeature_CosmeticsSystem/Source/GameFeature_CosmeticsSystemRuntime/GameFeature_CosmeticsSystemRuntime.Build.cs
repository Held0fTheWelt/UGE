using UnrealBuildTool;

public class GameFeature_CosmeticsSystemRuntime : ModuleRules
{
	public GameFeature_CosmeticsSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
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
				"Elemental_Interfaces",
				"ModularGameplay",
				"GameplayTags",
				"DeveloperSettings",
				"Slate",
				"SlateCore",
				"NetCore",  // FFastArraySerializer, replication
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
