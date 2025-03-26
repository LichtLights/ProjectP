// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Components/DBWeaponMasteryComponent.h"

#include "AbilitySystem/AbilityTree/WeaponMastery/DBWeaponMasteryNode.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBWeaponMasteryComponent)

UDBWeaponMasteryComponent::UDBWeaponMasteryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	AbilityPoints = 0;
}

void UDBWeaponMasteryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// UnregisterComponent();
}

bool UDBWeaponMasteryComponent::UnlockAbility(UDBWeaponMasteryNode* AbilityNode)
{
	if (!AbilityNode || UnlockedAbilities.Contains(AbilityNode))
	{
		return false; // Already unlocked or invalid
	}

	if (AbilityNode->CanUnlock(UnlockedAbilities) && AbilityPoints >= AbilityNode->AbilityPointCost)
	{
		AbilityPoints -= AbilityNode->AbilityPointCost;
		UnlockedAbilities.Add(AbilityNode);
		
		return true;
	}

	return false;
}

bool UDBWeaponMasteryComponent::LevelUpAbility(UDBWeaponMasteryNode* AbilityNode)
{
	if (!AbilityNode || !UnlockedAbilities.Contains(AbilityNode))
	{
		return false; // Not unlocked or invalid
	}

	// TODO: Implement.
	// if (AbilityNode->CanUnlock(UnlockedAbilities) && AbilityPoints >= AbilityNode->AbilityPointCost)
	// {
	// 	AbilityPoints -= AbilityNode->AbilityPointCost;
	// 	UnlockedAbilities.Add(AbilityNode);
	// 	return true;
	// }

	return false;
}

void UDBWeaponMasteryComponent::GrantUnlockedAbilities()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	UDBAbilitySystemComponent* ASC = Owner->FindComponentByClass<UDBAbilitySystemComponent>();
	if (!ASC) return;

	for (UDBWeaponMasteryNode* Node : UnlockedAbilities)
	{
		// TODO: Get ability level too.
		if (Node && Node->AbilityClass)
		{
			ASC->GiveAbility(FGameplayAbilitySpec(Node->AbilityClass, 1));
		}
	}
}
