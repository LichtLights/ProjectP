// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"

#include "DBHealExecution.generated.h"

class UObject;


/**
 *	UDBHealExecution
 *
 *	Execution used by gameplay effects to apply healing to the health attributes.
 */
UCLASS()
class UDBHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UDBHealExecution();

protected:
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
