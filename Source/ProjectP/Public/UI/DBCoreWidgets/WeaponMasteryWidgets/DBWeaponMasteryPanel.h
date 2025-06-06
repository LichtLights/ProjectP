// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DBCoreWidgets/DBGameMenuPanel.h"

#include "DBWeaponMasteryPanel.generated.h"

class UDBWeaponMasteryComponent;
class UDBAbilitySystemComponent;

/**
 *	UDBWeaponMasteryPanel
 *
 *	UCommonUserWidget which used as InGameMenu Panel.
 *	Is parent widget panel holds other widgets related to WeaponMastery.
 */
UCLASS()
class PROJECTP_API UDBWeaponMasteryPanel : public UDBGameMenuPanel
{
	GENERATED_BODY()

public:

	UDBWeaponMasteryPanel(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	UDBAbilitySystemComponent* GetDBAbilitySystemComponent();

	UFUNCTION(BlueprintCallable)
	UDBWeaponMasteryComponent* GetDBWeaponMasteryComponent();
};
