// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "DBDamageExecution.generated.h"

class UObject;


/**
 *	UDBDamageExecution
 *
 *	Execution used by gameplay effects to apply damage to the health attributes.
 */
UCLASS()
class PROJECTP_API UDBDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UDBDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
