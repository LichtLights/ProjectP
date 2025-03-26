// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "DBWeaponMasteryTree.generated.h"

class UDBWeaponMasteryNode;

/**
 * 
 */
UCLASS()
class PROJECTP_API UDBWeaponMasteryTree : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Mastery")
	TArray<UDBWeaponMasteryNode*> AbilityNodes;
};
