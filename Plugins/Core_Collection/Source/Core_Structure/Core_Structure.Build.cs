using UnrealBuildTool;

public class Core_Structure : ModuleRules
{
	public Core_Structure(ReadOnlyTargetRules Target) : base(Target)
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
				"Elemental_Structure",
			}
			);

		PrivateDependencyModuleNames.AddRange(new string[] { });
		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}
