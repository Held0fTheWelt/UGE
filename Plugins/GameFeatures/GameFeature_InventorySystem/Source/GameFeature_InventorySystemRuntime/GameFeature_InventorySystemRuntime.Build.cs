using UnrealBuildTool;

public class GameFeature_InventorySystemRuntime : ModuleRules
{
	public GameFeature_InventorySystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"EngineSettings",
				"Core_Interfaces",   // IInteractableTarget (in component class declaration)
				"Core_Interact",     // FInteractionOption (UPROPERTY in public header)
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Elemental_Interfaces",
				"Elemental_Player",
				"Elemental_Structure",
				"ModularGameplay",
				"GameplayAbilities",
				"GameplayTags",
				"GameplayMessageRuntime",
				"NetCore",
				"Core_Classes",      // UCombinedBundleActorComponent
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
