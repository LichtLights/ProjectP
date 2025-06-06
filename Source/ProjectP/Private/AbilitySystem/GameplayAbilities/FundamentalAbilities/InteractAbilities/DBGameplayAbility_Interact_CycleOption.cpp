// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/DBGameplayAbility_Interact_CycleOption.h"

#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameplayAbility_Interact_CycleOption)

UDBGameplayAbility_Interact_CycleOption::UDBGameplayAbility_Interact_CycleOption(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EDBAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UDBGameplayAbility_Interact_CycleOption::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Ensure we can commit the ability (i.e. spend any costs if applicable).
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Dispatch the gameplay event so that the active interaction ability (listening for TAG_Interaction_CycleOption)
	// can handle cycling the option.
	if (UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get())
	{
		const FGameplayTag TAG_Interaction_CycleOption = FGameplayTag::RequestGameplayTag(
			FName(TEXT("Ability.Interaction.CycleOption")));
		
		FGameplayEventData EventData;
		EventData.EventTag = TAG_Interaction_CycleOption;
		ASC->HandleGameplayEvent(TAG_Interaction_CycleOption, &EventData);
	}

	// End this ability as its sole job is to send the event.
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
