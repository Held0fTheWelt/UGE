using UnrealBuildTool;

public class Elemental_Editor : ModuleRules
{
    public Elemental_Editor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AssetRegistry",    
            "PropertyEditor",   
            "EditorStyle",      
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "Projects",
            "InputCore",
            "EditorFramework",
            "ToolMenus",
            "UnrealEd",        

            "ClassViewer",      
            
            "ContentBrowser",   

            "Slate",
            "SlateCore",

            "AssetLoader",
            "Elemental_Classes",
            "Elemental_Data",
            "Elemental_Interfaces",
            "Elemental_Structure",
            "UMG",
            "EditorWidgets"
        });

        DynamicallyLoadedModuleNames.AddRange(new string[]
        {
        });
    }
}
