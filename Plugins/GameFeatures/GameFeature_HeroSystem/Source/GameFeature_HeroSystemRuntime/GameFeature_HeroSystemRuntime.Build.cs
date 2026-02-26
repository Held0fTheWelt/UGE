// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameFeature_HeroSystemRuntime : ModuleRules
{
	public GameFeature_HeroSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"Elemental_Interfaces",
            }
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"ModularGameplay",
				"GameplayAbilities",
				"GameplayTags",
				"Elemental_Interfaces",
				"Elemental_Common",
			}
		);

		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
