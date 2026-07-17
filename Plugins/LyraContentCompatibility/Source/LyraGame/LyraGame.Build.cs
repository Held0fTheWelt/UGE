using UnrealBuildTool;

public class LyraGame : ModuleRules
{
    public LyraGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Engine", "UMG", "Slate", "SlateCore", "AIModule", "GameplayTags", "GameplayAbilities", "GameplayTasks", "GameFeatures",
            "ModularGameplay", "EnhancedInput", "CommonUI",
            "GameFeature_InventorySystemRuntime", "GameFeature_EquipmentSystemRuntime",
            "GameFeature_FeedbackSystemRuntime", "GameFeature_CosmeticsSystemRuntime", "Core_Classes"
        });
    }
}
