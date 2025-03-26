// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectP : ModuleRules
{
	public ProjectP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.AddRange(
			new string[] {
				"ProjectP/Public",
				"ProjectP/Core"
			}
		);

		PrivateIncludePaths.AddRange(
			new string[] {	
				"ProjectP/Private",
				"ProjectP/Core"
			}
		);
		
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core",
			"CoreUObject", "Engine", "InputCore", "PhysicsCore", "ApplicationCore",
			"Paper2D", "PaperZD", 
			"GameplayAbilities", "GameplayTags", "ModularGameplay", "ModularGameplayActors", "GameFeatures",
			"AsyncMixin",
			"Niagara",
			"GameplayTasks", "AIModule",
			"CommonLoadingScreen", "CommonStartupLoadingScreen", "ControlFlows",
			"NetCore", "Hotfix",
			
			"InventoryFrameworkPlugin", "IFP_GAS", "IFP_ItemQuery"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput", "CommonInput", "GameSettings",
			"Slate", "SlateCore", "CommonUI",
			"RenderCore",
			"UMG", "RHI", "UIExtension", "GameSubtitles",
			"GameplayMessageRuntime",
			"CommonGame", "CommonUser",
			"AudioModulation", "AudioMixer",
			"DeveloperSettings"
		});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
