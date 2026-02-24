// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UGEClientTarget : TargetRules
{
	public UGEClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Client;

		ExtraModuleNames.AddRange(new string[] { "UGE" });

        UGEGameTarget.ApplySharedTargetSettings(this);
	}
}
