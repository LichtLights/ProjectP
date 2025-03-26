// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/DBCombatSet.h"

#include "Net/UnrealNetwork.h"

UDBCombatSet::UDBCombatSet()
	: BaseDamage(0.0f)
	, BaseHeal(0.0f)
{
}

void UDBCombatSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDBCombatSet, BaseDamage, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDBCombatSet, BaseHeal, COND_OwnerOnly, REPNOTIFY_Always);
}

void UDBCombatSet::OnRep_BaseDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDBCombatSet, BaseDamage, OldValue);
}

void UDBCombatSet::OnRep_BaseHeal(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDBCombatSet, BaseHeal, OldValue);
}
