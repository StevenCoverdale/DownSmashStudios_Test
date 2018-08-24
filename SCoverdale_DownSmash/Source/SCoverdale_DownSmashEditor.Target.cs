// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SCoverdale_DownSmashEditorTarget : TargetRules
{
	public SCoverdale_DownSmashEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("SCoverdale_DownSmash");
	}
}
