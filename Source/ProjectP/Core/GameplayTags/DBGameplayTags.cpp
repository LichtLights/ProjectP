// Copyright Epic Games, Inc. All Rights Reserved.

#include "DBGameplayTags.h"

#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"
#include "LogChannel/DBLogChannels.h"

namespace DBGameplayTags
{
#pragma region ItemTags
	
	/** Item Tags */

	// Item Type Tags
	
	// Item Type Tags - Equipment
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Head, "IFP.Items.Type.Equipment.Head", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Torso, "IFP.Items.Type.Equipment.Torso", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Hands, "IFP.Items.Type.Equipment.Hands", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Feet, "IFP.Items.Type.Equipment.Feet", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Amulet, "IFP.Items.Type.Equipment.Amulet", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Belt, "IFP.Items.Type.Equipment.Belt", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Ring, "IFP.Items.Type.Equipment.Ring", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Ring_LeftRing, "IFP.Items.Type.Equipment.Ring.LeftRing", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Ring_RightRing, "IFP.Items.Type.Equipment.Ring.RightRing", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Weapon, "IFP.Items.Type.Equipment.Weapon", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Weapon_MainHand, "IFP.Items.Type.Equipment.Weapon.MainHand", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Weapon_OffHand, "IFP.Items.Type.Equipment.Weapon.OffHand", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Backpack, "IFP.Items.Type.Equipment.Backpack", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Equipment_Secured, "IFP.Items.Type.Equipment.Secured", "");

	// Item Type Tags - Consumable
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Consumable, "IFP.Items.Type.Consumable", "");

	// Item Type Tags - Barter
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Barter, "IFP.Items.Type.Barter", "");

	// Item Type Tags - Key
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Type_Key, "IFP.Items.Type.Key", "");

	
	// Item Status Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Status_IsNew, "IFP.Items.Status.IsNew", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Status_InHand, "IFP.Items.Status.InHand", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Items_Status_IsEquipped, "IFP.Items.Status.IsEquipped", "");
	
#pragma endregion

#pragma region ContainerTags
	
	/** Container Tags - Player */

	// Equipment Slot Tags - Player
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Head, "IFP.Containers.Identifier.Player.Head", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Torso, "IFP.Containers.Identifier.Player.Torso", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Hands, "IFP.Containers.Identifier.Player.Hands", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Feet, "IFP.Containers.Identifier.Player.Feet", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Amulet, "IFP.Containers.Identifier.Player.Amulet", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Belt, "IFP.Containers.Identifier.Player.Belt", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_LeftRing, "IFP.Containers.Identifier.Player.LeftRing", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_RightRing, "IFP.Containers.Identifier.Player.RightRing", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_MainHandWeapon, "IFP.Containers.Identifier.Player.MainHandWeapon", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_OffHandWeapon, "IFP.Containers.Identifier.Player.OffHandWeapon", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Backpack, "IFP.Containers.Identifier.Player.Backpack", "");

	// Standard Player Container Tags - Inventory
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Standard_0, "IFP.Containers.Identifier.Player.Standard.0", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Standard_1, "IFP.Containers.Identifier.Player.Standard.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Standard_2, "IFP.Containers.Identifier.Player.Standard.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_Standard_3, "IFP.Containers.Identifier.Player.Standard.3", "");

	// PocketGrid Player Container Tags - Inventory
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_0, "IFP.Containers.Identifier.Player.PocketGrid.0", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_1, "IFP.Containers.Identifier.Player.PocketGrid.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_2, "IFP.Containers.Identifier.Player.PocketGrid.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_3, "IFP.Containers.Identifier.Player.PocketGrid.3", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_4, "IFP.Containers.Identifier.Player.PocketGrid.4", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_PocketGrid_5, "IFP.Containers.Identifier.Player.PocketGrid.5", "");

	// DataOnly Player Container Tags - Inventory
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_DataOnly_0, "IFP.Containers.Identifier.Player.DataOnly.0", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_DataOnly_1, "IFP.Containers.Identifier.Player.DataOnly.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_DataOnly_2, "IFP.Containers.Identifier.Player.DataOnly.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Player_DataOnly_3, "IFP.Containers.Identifier.Player.DataOnly.3", "");

	/** Container Tags - Item */
	
	// Item Grid Container Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_0, "IFP.Containers.Identifier.Grid.0", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_1, "IFP.Containers.Identifier.Grid.1", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_2, "IFP.Containers.Identifier.Grid.2", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_3, "IFP.Containers.Identifier.Grid.3", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_4, "IFP.Containers.Identifier.Grid.4", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_5, "IFP.Containers.Identifier.Grid.5", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_6, "IFP.Containers.Identifier.Grid.6", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_7, "IFP.Containers.Identifier.Grid.7", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_8, "IFP.Containers.Identifier.Grid.8", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Containers_Identifier_Grid_9, "IFP.Containers.Identifier.Grid.9", "");
	
#pragma endregion

#pragma region TileTags
	
	// Tile Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Tiles_Hidden, "IFP.Tiles.Hidden", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(IFP_Tiles_Locked, "IFP.Tiles.Locked", "");
	
#pragma endregion

#pragma region AbilityTags

	// Interaction Ability Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_CycleTarget, "Ability.Interaction.CycleTarget", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_CycleOption, "Ability.Interaction.CycleOption", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate, "Ability.Interaction.Activate", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_Collect, "Ability.Interaction.Activate.Collect", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_LootBox, "Ability.Interaction.Activate.LootBox", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_UnlockBox, "Ability.Interaction.Activate.UnlockBox", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_OpenDoor, "Ability.Interaction.Activate.OpenDoor", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_CloseDoor, "Ability.Interaction.Activate.CloseDoor", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Activate_UnlockDoor, "Ability.Interaction.Activate.UnlockDoor", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Interaction_Duration_Message, "Ability.Interaction.Duration.Message", "");
	
#pragma endregion

#pragma region LyraBase
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_IsDead, "Ability.ActivateFail.IsDead", "Ability failed to activate because its owner is dead.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cooldown, "Ability.ActivateFail.Cooldown", "Ability failed to activate because it is on cool down.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Cost, "Ability.ActivateFail.Cost", "Ability failed to activate because it did not pass the cost checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsBlocked, "Ability.ActivateFail.TagsBlocked", "Ability failed to activate because tags are blocking it.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_TagsMissing, "Ability.ActivateFail.TagsMissing", "Ability failed to activate because tags are missing.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_Networking, "Ability.ActivateFail.Networking", "Ability failed to activate because it did not pass the network checks.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ActivateFail_ActivationGroup, "Ability.ActivateFail.ActivationGroup", "Ability failed to activate because of its activation group.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Behavior_SurvivesDeath, "Ability.Behavior.SurvivesDeath", "An ability with this type tag should not be canceled due to death.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Crouch, "InputTag.Crouch", "Crouch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_AutoRun, "InputTag.AutoRun", "Auto-run input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_Spawned, "InitState.Spawned", "1: Actor/component has initially spawned and can be extended");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataAvailable, "InitState.DataAvailable", "2: All required data has been loaded/replicated and is ready for initialization");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_DataInitialized, "InitState.DataInitialized", "3: The available data has been initialized for this actor/component, but it is not ready for full gameplay");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InitState_GameplayReady, "InitState.GameplayReady", "4: The actor/component is fully ready for active gameplay");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Death, "GameplayEvent.Death", "Event that fires on death. This event only fires on the server.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Reset, "GameplayEvent.Reset", "Event that fires once a player reset is executed.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_RequestReset, "GameplayEvent.RequestReset", "Event to request a player's pawn to be instantly replaced with a new one at a valid spawn location.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Damage, "SetByCaller.Damage", "SetByCaller tag used by damage gameplay effects.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SetByCaller_Heal, "SetByCaller.Heal", "SetByCaller tag used by healing gameplay effects.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_GodMode, "Cheat.GodMode", "GodMode cheat is active on the owner.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Cheat_UnlimitedHealth, "Cheat.UnlimitedHealth", "UnlimitedHealth cheat is active on the owner.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Crouching, "Status.Crouching", "Target is crouching.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_AutoRunning, "Status.AutoRunning", "Target is auto-running.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death, "Status.Death", "Target has the death status.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dying, "Status.Death.Dying", "Target has begun the death process.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Death_Dead, "Status.Death.Dead", "Target has finished the death process.");
						  
	// These are mapped to the movement modes inside GetMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Walking, "Movement.Mode.Walking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_NavWalking, "Movement.Mode.NavWalking", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Falling, "Movement.Mode.Falling", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Swimming, "Movement.Mode.Swimming", "Default Character movement tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Flying, "Movement.Mode.Flying", "Default Character movement tag");

	// When extending Lyra, you can create your own movement modes but you need to update GetCustomMovementModeTagMap()
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Movement_Mode_Custom, "Movement.Mode.Custom", "This is invalid and should be replaced with custom tags.  See LyraGameplayTags::CustomMovementModeTagMap.");

#pragma endregion
	
	// Unreal Movement Modes
	const TMap<uint8, FGameplayTag> MovementModeTagMap =
	{
		{ MOVE_Walking, Movement_Mode_Walking },
		{ MOVE_NavWalking, Movement_Mode_NavWalking },
		{ MOVE_Falling, Movement_Mode_Falling },
		{ MOVE_Swimming, Movement_Mode_Swimming },
		{ MOVE_Flying, Movement_Mode_Flying },
		{ MOVE_Custom, Movement_Mode_Custom }
	};

	// Custom Movement Modes
	const TMap<uint8, FGameplayTag> CustomMovementModeTagMap =
	{
		// Fill these in with your custom modes
	};

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogDB, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}

		return Tag;
	}
}

