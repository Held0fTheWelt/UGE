// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.Server)]
public class UGEServerTarget : TargetRules
{
	public UGEServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;

		ExtraModuleNames.AddRange(new string[] { "UGE" });

        UGEGameTarget.ApplySharedTargetSettings(this);

		bUseChecksInShipping = true;
	}
}
