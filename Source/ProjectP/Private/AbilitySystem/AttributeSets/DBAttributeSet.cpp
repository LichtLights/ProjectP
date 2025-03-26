// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/DBAttributeSet.h"

#include "AbilitySystem/Components/DBAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBAttributeSet)

UDBAttributeSet::UDBAttributeSet()
{
}

UWorld* UDBAttributeSet::GetWorld() const
{
	// return Super::GetWorld();
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UDBAbilitySystemComponent* UDBAttributeSet::GetAbilitySystemComponent() const
{
	return Cast<UDBAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}
