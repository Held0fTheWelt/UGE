using UnrealBuildTool;
 
public class Core_Interact : ModuleRules
{
	public Core_Interact(ReadOnlyTargetRules Target) : base(Target)
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
                "Core_AbilitySystem",             // UCoreAbilitySystemComponent, UCoreGameplayAbility, FCoreGameplayEffectContext
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                                "GameplayAbilities",
                                                "GameplayTags",
                "GameplayTasks",
                "Slate",
                "SlateCore",
                "Core_Interfaces",           // ICoreSettingsLocalPlayerInterface, CoreGameInstanceDelegates, PerformanceStat provider
                "UMG",
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