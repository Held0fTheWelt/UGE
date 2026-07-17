using UnrealBuildTool;

public class ShooterCoreRuntime : ModuleRules
{
    public ShooterCoreRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "EnhancedInput", "GameplayTags"
        });
    }
}
