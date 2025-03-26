// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/BaseAbility.h"

UBaseAbility::UBaseAbility()
{
	// Default init
	AbilityName = FText::FromString("Unnamed Ability");
	AbilityDescription = FText::FromString("No description");
	AbilityType = EAbilityType::Undetermined;
	MaxMasterLevel = 1;
	CurrentLevel = 0;
	AbilityPointCost = 1;
}

bool UBaseAbility::ArePrerequisitesMet(const TArray<UBaseAbility*>& LearnedAbilities) const
{
	for (UBaseAbility* Prerequisite : PrerequisiteAbilities)
	{
		if (!LearnedAbilities.Contains(Prerequisite) || Prerequisite->CurrentLevel == 0)
		{
			// If any prerequisite is not learned, return false
			return false;
		}
	}
	return true;
}

void UBaseAbility::UpgradeAbility()
{
	if (CurrentLevel < MaxMasterLevel)
	{
		CurrentLevel++;
		ApplyAbilityEffect();
	}
}

void UBaseAbility::ApplyAbilityEffect()
{
	// This will be implemented differently for Active and Passive abilities.
	
}
