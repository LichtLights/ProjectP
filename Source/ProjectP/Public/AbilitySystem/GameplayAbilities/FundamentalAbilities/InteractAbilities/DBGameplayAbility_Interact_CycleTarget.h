// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/DBGameplayAbility.h"

#include "DBGameplayAbility_Interact_CycleTarget.generated.h"

/**
 *	A dedicated ability that dispatches the "cycle target" gameplay event.
 */
UCLASS()
class PROJECTP_API UDBGameplayAbility_Interact_CycleTarget : public UDBGameplayAbility
{
	GENERATED_BODY()

public:

	UDBGameplayAbility_Interact_CycleTarget(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
