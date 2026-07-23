// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InterInvSystem : ModuleRules
{
	public InterInvSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"InterInvSystem",
			"InterInvSystem/Variant_Platforming",
			"InterInvSystem/Variant_Platforming/Animation",
			"InterInvSystem/Variant_Combat",
			"InterInvSystem/Variant_Combat/AI",
			"InterInvSystem/Variant_Combat/Animation",
			"InterInvSystem/Variant_Combat/Gameplay",
			"InterInvSystem/Variant_Combat/Interfaces",
			"InterInvSystem/Variant_Combat/UI",
			"InterInvSystem/Variant_SideScrolling",
			"InterInvSystem/Variant_SideScrolling/AI",
			"InterInvSystem/Variant_SideScrolling/Gameplay",
			"InterInvSystem/Variant_SideScrolling/Interfaces",
			"InterInvSystem/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
