// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystem/GameplayAbilities/DBGameplayAbility.h"

#include "DBGameplayAbility_BaseMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UDBGameplayAbility_BaseMeleeAttack : public UDBGameplayAbility
{
	GENERATED_BODY()

public:

	UDBGameplayAbility_BaseMeleeAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UGameplayAbility interface
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface
};
