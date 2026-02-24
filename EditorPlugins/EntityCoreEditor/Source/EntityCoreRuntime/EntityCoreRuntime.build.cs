using UnrealBuildTool;
 
public class EntityCoreRuntime : ModuleRules
{
	public EntityCoreRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}