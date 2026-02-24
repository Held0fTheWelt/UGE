// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Controllers : ModuleRules
{
	public Controllers(ReadOnlyTargetRules Target) : base(Target)
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
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AIModule",           // IGenericTeamAgentInterface, FGenericTeamId
				"EngineSettings",     // UGameMapsSettings::GetGameDefaultMap
				"Slate",
				"SlateCore",
				"ApplicationCore",
                "InputCore",
                "InputSystem",
                "EnhancedInput",
                "LoggingMacros",
                "AssetLoader",
                "Elemental_Classes",
                "Elemental_Common",
                "Elemental_Interfaces",
                "Elemental_Logging",
                "Elemental_Structure",
				"Elemental_Player",
				"CommonInput",
                "GameplayAbilities",
                "GameplayTags",  // FGameplayTag::RequestGameplayTag for Status.AutoRunning
				"NetCore",
				//"ReplaySubsystem",
				// ... add private dependencies that you statically link with here ...	
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
