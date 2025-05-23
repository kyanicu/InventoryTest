// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventoryTest : ModuleRules
{
	public InventoryTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
		PublicIncludePaths.AddRange(new string[] { "InventoryTest/Source/InventoryTest/Public" });
	}
}
