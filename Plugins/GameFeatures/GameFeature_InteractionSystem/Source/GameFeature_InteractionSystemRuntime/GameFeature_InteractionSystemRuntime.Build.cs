using UnrealBuildTool;

public class GameFeature_InteractionSystemRuntime : ModuleRules
{
	public GameFeature_InteractionSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"GameplayAbilities",
				"GameplayTags",
				"GameplayTasks",
				"Core_Interfaces",
				"Core_Interact",
				//"NativeGameplayTags",
				"UMG",
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
