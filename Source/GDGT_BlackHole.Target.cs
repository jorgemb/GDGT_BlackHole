// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class GDGT_BlackHoleTarget : TargetRules
{
	public GDGT_BlackHoleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "GDGT_BlackHole" } );
	}
}
