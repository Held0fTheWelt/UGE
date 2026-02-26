// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection

using UnrealBuildTool;

public class GameFeature_EquipmentSystemRuntime : ModuleRules
{
	public GameFeature_EquipmentSystemRuntime(ReadOnlyTargetRules Target) : base(Target)
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
				"Niagara",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GameplayAbilities",
				"GameplayTags",
				"IrisCore",
				"ModularGameplay",
				"NetCore",
				"Elemental_Interfaces",
				"Elemental_MessageSystem",
				"GameplayMessageRuntime",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
