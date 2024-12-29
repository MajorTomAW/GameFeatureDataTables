// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameFeatureDataTables : ModuleRules
{
	public GameFeatureDataTables(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new[] 
		{ 
			"Core",
			"GameFeatures"
		});
			
		
		PrivateDependencyModuleNames.AddRange(new[]
		{ 
			"CoreUObject", 
			"Engine",
		});
	}
}
