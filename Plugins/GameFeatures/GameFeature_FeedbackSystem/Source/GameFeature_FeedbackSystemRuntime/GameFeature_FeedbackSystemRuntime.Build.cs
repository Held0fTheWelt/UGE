// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameFeature_FeedbackSystemRuntime : ModuleRules
{
	public GameFeature_FeedbackSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"ModularGameplay",
			}
			);
			
		
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				"DeveloperSettings",
				"GameplayTags",
				"Niagara",
				"AudioMixer",
				"PhysicsCore",
				"Elemental_Interfaces",
            }
            );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
