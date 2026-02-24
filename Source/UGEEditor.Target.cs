// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UGEEditorTarget : TargetRules
{
	public UGEEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("UGE");

        if (!bBuildAllModules)
        {
            NativePointerMemberBehaviorOverride = PointerMemberBehavior.AllowSilently;
        }
        UGEGameTarget.ApplySharedTargetSettings(this);

        // This is used for touch screen development along with the "Unreal Remote 2" app
        EnablePlugins.Add("RemoteSession");
    }
}
