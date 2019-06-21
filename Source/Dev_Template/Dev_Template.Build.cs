// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dev_Template : ModuleRules
{
	public Dev_Template(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "CableComponent" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "CableComponent" });
	}
}
