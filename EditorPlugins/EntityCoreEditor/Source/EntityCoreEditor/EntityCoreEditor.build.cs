using UnrealBuildTool;
 
public class EntityCoreEditor : ModuleRules
{
	public EntityCoreEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "EditorScriptingUtilities",
                "ScriptableEditorWidgets",
                "UMG",
                "EditorManager",
                "PropertyEditor",
                "Blutility",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Projects",
                "InputCore",
                "EditorFramework",
                "UnrealEd",
                "ToolMenus",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "DataValidation",
                "LoggingMacros",


                "AssetLoader",
                "Elemental_Data",
                "Core_Data",
                "Aircrafts",
                "Helicopters",
                //"Warfare",
                //"Warfare_Train",
                "UMGEditor",
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