// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DA_DBInteractAbilities.h"
#include "GameplayTagsSettings.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GameplayAbilities/DBGameplayAbility.h"
#include "GameplayTags/DBGameplayTags.h"
#include "InteractionOptions.generated.h"

class UDA_DBInteractAbilities;
class IInteractableTarget;
class UUserWidget;

UENUM(BlueprintType)
enum class EInteractionOptionType : uint8
{
	Unknown = 0,

	Collect,

	Loot_Box,
	Unlock_Box,

	Open_Door,
	Close_Door,
	Unlock_Door
};

/**  */
USTRUCT(BlueprintType)
struct FInteractionOption
{
	GENERATED_BODY()

public:
	
	/** The interactable target */
	UPROPERTY(BlueprintReadWrite)
	TScriptInterface<IInteractableTarget> InteractableTarget;

	/** Simple text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text;

	/** Simple sub-text the interaction might return */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SubText;

	/** The type of interaction this is. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInteractionOptionType InteractionOptionType = EInteractionOptionType::Unknown;

	/** The tag for this interaction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag InteractionGameplayTag;

	/** The ability to grant the avatar when they get near interactable objects. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;

	// // METHODS OF INTERACTION
	// //--------------------------------------------------------------
	//
	// // 1) Place an ability on the avatar that they can activate when they perform interaction.
	//
	// /** The ability to grant the avatar when they get near interactable objects. */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// TSubclassOf<UGameplayAbility> InteractionAbilityToGrant;
	//
	// // - OR -
	//
	// // 2) Allow the object we're interacting with to have its own ability system and interaction ability, that we can activate instead.
	//
	// /** The ability system on the target that can be used for the TargetInteractionHandle and sending the event, if needed. */
	// UPROPERTY(BlueprintReadOnly)
	// TObjectPtr<UAbilitySystemComponent> TargetAbilitySystem = nullptr;
	//
	// /** The ability spec to activate on the object for this option. */
	// UPROPERTY(BlueprintReadOnly)
	// FGameplayAbilitySpecHandle TargetInteractionAbilityHandle;
	//
	// // UI
	// //--------------------------------------------------------------
	//
	// /** The widget to show for this kind of interaction. */
	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// TSoftClassPtr<UUserWidget> InteractionWidgetClass;
	//
	// //--------------------------------------------------------------


	static FInteractionOption CreateInteractionOption(EInteractionOptionType Type, TScriptInterface<IInteractableTarget> Target);

	static void SortOptionsByAlphabeticalOrder(TArray<FInteractionOption>& Options);


public:

	// "==" operator checks whether the two options are exactly the same instance
	// FORCEINLINE bool operator==(const FInteractionOption& Other) const
	// {
	// 	return InteractableTarget == Other.InteractableTarget &&
	// 		Text.IdenticalTo(Other.Text) &&
	// 		SubText.IdenticalTo(Other.SubText) &&
	// 		InteractionOptionType == Other.InteractionOptionType;
	// }

	// Rather use IsSemanticallyEqual than "==" when comparing options
	// FORCEINLINE bool IsSemanticallyEqual(const FInteractionOption& Other) const
	// {
	// 	return InteractableTarget.GetObject() == Other.InteractableTarget.GetObject() &&
	// 		Text.ToString() == Other.Text.ToString() &&
	// 		SubText.ToString() == Other.SubText.ToString() &&
	// 		InteractionOptionType == Other.InteractionOptionType;
	// }

	// "==" operator checks whether the two options are can be considered as same option.
	FORCEINLINE bool operator==(const FInteractionOption& Other) const
	{
		return InteractableTarget.GetObject() == Other.InteractableTarget.GetObject() &&
			Text.ToString() == Other.Text.ToString() &&
			SubText.ToString() == Other.SubText.ToString() &&
			InteractionOptionType == Other.InteractionOptionType;
	}

	// IsSameInstance() checks whether the two options are exactly the same instance
	FORCEINLINE bool IsSameInstance(const FInteractionOption& Other) const
	{
		return InteractableTarget == Other.InteractableTarget &&
			Text.IdenticalTo(Other.Text) &&
			SubText.IdenticalTo(Other.SubText) &&
			InteractionOptionType == Other.InteractionOptionType;
	}

	FORCEINLINE bool operator!=(const FInteractionOption& Other) const
	{
		return !operator==(Other);
	}

	FORCEINLINE bool operator<(const FInteractionOption& Other) const
	{
		return InteractableTarget.GetInterface() < Other.InteractableTarget.GetInterface();
	}
	
	
	
};

inline FInteractionOption FInteractionOption::CreateInteractionOption(const EInteractionOptionType Type, TScriptInterface<IInteractableTarget> Target)
{
	FInteractionOption Option = FInteractionOption();

	Option.InteractableTarget = Target;

	UDA_DBInteractAbilities* InteractAbilities = Cast<UDA_DBInteractAbilities>(
		StaticLoadObject(UDA_DBInteractAbilities::StaticClass(),
		                 nullptr,
		                 TEXT("/Game/ProjectP/System/AbilitySystem/Abilities/FundamentalAbilities/InteractAbilities/DA_InteractAbilitiesToGrant.DA_InteractAbilitiesToGrant"))
	);
	
	switch (Type)
	{
	case EInteractionOptionType::Unknown:

		Option.Text = FText::FromString("Unknown");
		Option.InteractionOptionType = EInteractionOptionType::Unknown;
		break;
		

	//~Item interactions
	case EInteractionOptionType::Collect:

		Option.Text = FText::FromString("Collect");
		Option.InteractionOptionType = EInteractionOptionType::Collect;
		// Option.InteractionGameplayTag = FGameplayTag::RequestGameplayTag(FName(TEXT("InputTag.Ability.Interact.Collect")));
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_Collect;

		Option.InteractionAbilityToGrant = InteractAbilities->Interact_Collect;
		break;
	//~End of Item interactions


	// LootBox interactions
	case EInteractionOptionType::Loot_Box:

		Option.Text = FText::FromString("Loot");
		Option.InteractionOptionType = EInteractionOptionType::Loot_Box;
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_LootBox;
		
		Option.InteractionAbilityToGrant = InteractAbilities->Interact_LootBox;
		break;

	case EInteractionOptionType::Unlock_Box:
		
		Option.Text = FText::FromString("Unlock");
		Option.InteractionOptionType = EInteractionOptionType::Unlock_Box;
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_UnlockBox;
		
		Option.InteractionAbilityToGrant = InteractAbilities->Interact_UnlockBox;
		break;
	//~End of LootBox interactions

		
	// Door interactions
	case EInteractionOptionType::Open_Door:

		Option.Text = FText::FromString("Open door");
		Option.InteractionOptionType = EInteractionOptionType::Open_Door;
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_OpenDoor;

		Option.InteractionAbilityToGrant = InteractAbilities->Interact_OpenDoor;
		break;

	case EInteractionOptionType::Close_Door:

		Option.Text = FText::FromString("Close door");
		Option.InteractionOptionType = EInteractionOptionType::Close_Door;
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_CloseDoor;
		
		Option.InteractionAbilityToGrant = InteractAbilities->Interact_CloseDoor;
		break;

	case EInteractionOptionType::Unlock_Door:
		
		Option.Text = FText::FromString("Unlock door");
		Option.InteractionOptionType = EInteractionOptionType::Unlock_Door;
		Option.InteractionGameplayTag = DBGameplayTags::Ability_Interaction_Activate_UnlockDoor;
		
		Option.InteractionAbilityToGrant = InteractAbilities->Interact_UnlockDoor;
		break;
	//~End of Door interactions

		
	
	default:

		Option.Text = FText::FromString("Not defined");
		Option.InteractionOptionType = EInteractionOptionType::Unknown;
		break;
	}
	
	

	return Option;
}

inline void FInteractionOption::SortOptionsByAlphabeticalOrder(TArray<FInteractionOption>& Options)
{
	Options.Sort([](const FInteractionOption& A, const FInteractionOption& B)
	{
		// Convert FText to FString for comparison.
		return A.Text.ToString() < B.Text.ToString();
	});
}
