// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseAbility.h"
#include "PassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UPassiveAbility : public UBaseAbility
{
	GENERATED_BODY()

protected:
	
	virtual void ApplyAbilityEffect() override;
	
};
