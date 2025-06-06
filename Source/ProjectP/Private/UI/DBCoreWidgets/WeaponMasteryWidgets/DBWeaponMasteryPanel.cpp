// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBCoreWidgets/WeaponMasteryWidgets/DBWeaponMasteryPanel.h"

#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "AbilitySystem/Components/DBWeaponMasteryComponent.h"
#include "Characters/PlayerCharacters/DBPlayerCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBWeaponMasteryPanel)

UDBWeaponMasteryPanel::UDBWeaponMasteryPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UDBAbilitySystemComponent* UDBWeaponMasteryPanel::GetDBAbilitySystemComponent()
{
	ACharacter* PC = GetOwningPlayer()->GetCharacter();
	if (!PC) return nullptr;

	UDBAbilitySystemComponent* ASC = PC->FindComponentByClass<UDBAbilitySystemComponent>();

	return ASC;
}

UDBWeaponMasteryComponent* UDBWeaponMasteryPanel::GetDBWeaponMasteryComponent()
{
	ACharacter* PC = GetOwningPlayer()->GetCharacter();
	if (!PC) return nullptr;

	UDBWeaponMasteryComponent* WMC = PC->FindComponentByClass<UDBWeaponMasteryComponent>();

	return WMC;
}
