// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UdemyMultiplayer : ModuleRules
{
	public UdemyMultiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"EnhancedInput", 
			"OnlineSubsystem", 
			"SlateCore",
			"MoviePlayer"
		});

        PrivateDependencyModuleNames.AddRange(
                new string[] {
                "UMG",
                "Slate",
				"CommonUI"
                });
    }
}
