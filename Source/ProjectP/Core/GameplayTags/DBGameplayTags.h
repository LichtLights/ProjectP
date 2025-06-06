// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"

namespace DBGameplayTags
{
	PROJECTP_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

#pragma region ItemTags
	/** Item Tags */

	// Item Type Tags - Equipment
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Head);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Torso);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Hands);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Feet);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Amulet);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Belt);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Ring);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Ring_LeftRing);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Ring_RightRing);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Weapon);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Weapon_MainHand);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Weapon_OffHand);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Backpack);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Equipment_Secured);

	// Item Type Tags - Consumable
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Consumable);

	// Item Type Tags - Barter
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Barter)

	// Item Type Tags - Key
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Type_Key)
	

	// Item Status Tags
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Status_IsNew);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Status_InHand);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Items_Status_IsEquipped);


#pragma endregion

#pragma region ContainerTags
	/** Container Tags - Player inherits */

	// Equipment Slot Tags - Player
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Head);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Torso);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Hands);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Feet);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Amulet);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Belt);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_LeftRing);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_RightRing);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_MainHandWeapon);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_OffHandWeapon);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Backpack);	
	
	// Standard Player Container Tags - Inventory
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Standard_0);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Standard_1);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Standard_2);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_Standard_3);

	// PocketGrid Player Container Tags - Inventory
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_0);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_1);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_2);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_3);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_4);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_PocketGrid_5);

	// DataOnly Player Container Tags - Inventory
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_DataOnly_0);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_DataOnly_1);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_DataOnly_2);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Player_DataOnly_3);

	
	/** Container Tags - Item */
	
	// Item Grid Container Tags
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_0);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_1);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_2);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_3);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_4);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_5);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_6);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_7);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_8);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Containers_Identifier_Grid_9);
#pragma endregion	

#pragma region TileTags
	
	// Tile Tags
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Tiles_Hidden);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(IFP_Tiles_Locked);

#pragma endregion

#pragma region AbilityTags

	// Interaction Ability Tags
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_CycleTarget);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_CycleOption);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_Collect);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_LootBox);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_UnlockBox);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_OpenDoor);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_CloseDoor);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Activate_UnlockDoor);
	
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Interaction_Duration_Message);

#pragma endregion
	

#pragma region LyraBase
	// Declare all of the custom native tags that Lyra will use
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_AutoRun);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_Spawned);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataAvailable);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_DataInitialized);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InitState_GameplayReady);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Damage);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetByCaller_Heal);

	// TODO: Remove Cheat tags.
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_GodMode);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cheat_UnlimitedHealth);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_AutoRunning);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	PROJECTP_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	PROJECTP_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	PROJECTP_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
#pragma endregion

};
