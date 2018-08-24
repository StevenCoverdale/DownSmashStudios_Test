// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SCoverdale_DownSmashTarget : TargetRules
{
	public SCoverdale_DownSmashTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("SCoverdale_DownSmash");
	}
}
