// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTree/WeaponMastery/DBWeaponMasteryNode.h"

#include "AbilitySystem/GameplayAbilities/DBGameplayAbility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBWeaponMasteryNode)

bool UDBWeaponMasteryNode::IsAbilityAssigned()
{
	if (IsValid(AbilityClass))
	{
		return true;
	}
	return false;
}

bool UDBWeaponMasteryNode::CanUnlock(const TArray<UDBWeaponMasteryNode*>& UnlockedAbilities) const
{
	for (UDBWeaponMasteryNode* Prereq : Prerequisites)
	{
		if (!UnlockedAbilities.Contains(Prereq))
		{
			return false; // A prerequisite ability is missing
		}
	}
	return true;
}

void UDBWeaponMasteryNode::AssignAbilityToNode(TSubclassOf<UDBGameplayAbility> InAbility)
{
	if (IsValid(InAbility))
	{
		// TODO: unregister former ability from AbilityComponent.
		AbilityClass = InAbility;
	}

	UE_LOG(LogTemp, Error, TEXT("InAbility is not valid at UDBWeaponMasteryNode::AssignAbilityToNode()"));
	return;
}
