// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "ActiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UActiveAbility : public UBaseAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ApplyAbilityEffect() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void UseAbility();
};
