// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"

#include "DBWeaponMasteryNode.generated.h"

class UDBGameplayAbility;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTP_API UDBWeaponMasteryNode : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSubclassOf<UDBGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	int32 AbilityPointCost = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Tree")
	TArray<UDBWeaponMasteryNode*> Prerequisites;

	UFUNCTION(BlueprintCallable)
	bool IsAbilityAssigned();
	UFUNCTION(BlueprintCallable)
	void ClearAbility() { AbilityClass = nullptr; }
	
	
	UFUNCTION(BlueprintCallable, Category="Mastery")
	bool CanUnlock(const TArray<UDBWeaponMasteryNode*>& UnlockedAbilities) const;

	UFUNCTION(BlueprintCallable)
	void AssignAbilityToNode(TSubclassOf<UDBGameplayAbility> InAbility);
	
	
};
