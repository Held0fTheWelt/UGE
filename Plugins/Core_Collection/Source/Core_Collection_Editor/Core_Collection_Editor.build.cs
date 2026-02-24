// Copyright Epic Games, Inc. All Rights Reserved.
// Adapted for WarCollection / Core

using UnrealBuildTool;

public class Core_Collection_Editor : ModuleRules
{
	public Core_Collection_Editor(ReadOnlyTargetRules Target) : base(Target)
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
				"CoreUObject",
				"Engine",
				"AssetRegistry",
				"PropertyEditor",
				"EditorStyle",
				"EditorFramework",
				"UnrealEd",
				"PhysicsCore",
				"GameplayTagsEditor",
				"GameplayTasksEditor",
				"GameplayAbilities",
				"GameplayAbilitiesEditor",
				"StudioTelemetry",
				"Core_Classes",
				"Core_Development",
				"Core_System",
				}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"InputCore",
				"LevelEditor",
				"AssetLoader",
				"InteractiveToolsFramework",
				"EditorInteractiveToolsFramework",
				"ToolMenus",
				"DataValidation",
				"MessageLog",
				"Projects",
				"DeveloperToolSettings",
				"CollectionManager",
				"SourceControl",
				"Chaos",
				"AssetTools",
				"Core_AbilitySystem",
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
