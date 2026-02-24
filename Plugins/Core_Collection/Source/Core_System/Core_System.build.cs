using UnrealBuildTool;
 
public class Core_System : ModuleRules
{
	public Core_System(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
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
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "GameplayAbilities",
                "DeveloperSettings",
                "SignificanceManager",
                "ReplicationGraph",
                "ModularGameplay",
                "GameplayTags",
                "CommonUser",
                "AssetLoader",
                "Core_AbilitySystem",      // UAbilityTagRelationshipMapping (PawnData_Core)
                "Elemental_Common",
                "Elemental_Interfaces",
                "Elemental_Player",         // APlayerState_Elemental in CorePawnDataProviderSubsystem
                "Core_Structure",
                "Core_Interfaces",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );

        SetupGameplayDebuggerSupport(Target);
    }
}