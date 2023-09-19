// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ReviewProject : ModuleRules
{
	public ReviewProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", 
			"OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam",
			"GameplayAbilities", "GameplayTags", "GameplayTasks", 
			"SlateCore", "Slate", "UMG" });
	}
}
