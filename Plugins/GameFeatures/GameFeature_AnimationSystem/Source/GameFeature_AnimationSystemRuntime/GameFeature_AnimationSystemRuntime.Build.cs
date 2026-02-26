using UnrealBuildTool;

public class GameFeature_AnimationSystemRuntime : ModuleRules
{
	public GameFeature_AnimationSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"AnimationCore",
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Elemental_Interfaces",
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
