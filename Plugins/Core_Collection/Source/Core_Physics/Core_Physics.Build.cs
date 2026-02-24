using UnrealBuildTool;
 
public class Core_Physics : ModuleRules
{
	public Core_Physics(ReadOnlyTargetRules Target) : base(Target)
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
                "PhysicsCore",
                "Core_Interfaces",   // IPhysicalMaterialWithTagsInterface
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Elemental_Interfaces",
                "GameplayTags",
            }
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
            }
            );
    }
}
