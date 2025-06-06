// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DBAbilitySystemGlobals.h"

#include "AbilitySystem/DBGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBAbilitySystemGlobals)

struct FGameplayEffectContext;

UDBAbilitySystemGlobals::UDBAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FGameplayEffectContext* UDBAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDBGameplayEffectContext();
}
