// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ActiveAbilities/ActiveAbility.h"

void UActiveAbility::ApplyAbilityEffect()
{
	// Increase the power, reduce cooldown, etc.
	UE_LOG(LogTemp, Warning, TEXT("Active ability power increased"));
}

void UActiveAbility::UseAbility()
{
	if (CurrentLevel > 0)
	{
		// Trigger the ability in the game (e.g., casting a spell, dealing damage)
		UE_LOG(LogTemp, Warning, TEXT("Active ability used"));
	}
}
