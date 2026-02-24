using UnrealBuildTool;
 
public class Core_AbilitySystem : ModuleRules
{
	public Core_AbilitySystem(ReadOnlyTargetRules Target) : base(Target)
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
                "Core_Structure",
                "Elemental_Interfaces",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core_Interfaces",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayAbilities",
                "GameplayTags",
                "GameplayTasks",
                "EngineSettings",  // For UPrimaryDataAsset
                "NetCore",          // For replication
                "IrisCore",        // For UE_WITH_IRIS NetSerializer (CoreGameplayEffectContext)
                "Elemental_Structure",      // For FVerbMessage
                "Elemental_Player",          // APlayerState_Elemental for CoreAbilityCost_PlayerTagStack
                "GameplayMessageRuntime",    // For GameplayMessageSubsystem (plugin: CorePlugins/GameplayMessageRouter)
                // Note: Physical material tags via IPhysicalMaterialWithTagsInterface (Core_Interfaces); Core_Physics implements it.
                // Note: GameFeature_InventorySystemRuntime is not directly included to avoid circular dependency
                // The InventoryItem cost uses runtime class lookup instead
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