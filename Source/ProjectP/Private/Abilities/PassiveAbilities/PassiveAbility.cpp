// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/PassiveAbilities/PassiveAbility.h"

void UPassiveAbility::ApplyAbilityEffect()
{
	// Apply permanent buff to the character stats (e.g., increase health, mana, etc.)
	UE_LOG(LogTemp, Warning, TEXT("Passive ability effect applied"));
}
