// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionQuery.h"
#include "AbilityTask_ScanForInteractableTargets.generated.h"

class AActor;
struct FInteractionOption;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScanForTargetsDelegate, TArray<FInteractionOption>&, InteractionOptions);

/**
 *	Ability task that periodically scans for nearby interactable targets
 *	and gathers their interaction options.
 */
UCLASS()
class UAbilityTask_ScanForInteractableTargets : public UAbilityTask
{
	GENERATED_BODY()

public:
	
	UAbilityTask_ScanForInteractableTargets(const FObjectInitializer& ObjectInitializer);

	/** Wait until we trace new set of interactables.  This task automatically loops. */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_ScanForInteractableTargets* ScanForInteractableTargets
	(
		UGameplayAbility* OwningAbility,
		FGameplayAbilityTargetingLocationInfo StartLocation,
		float InteractionScanRadius = 100,
		float InteractionScanRate = 0.100,
		bool bShowDebug = false
	);
	
	/** Delegate called every scan with the gathered interaction options. */
	UPROPERTY(BlueprintAssignable)
	FOnScanForTargetsDelegate OnScanForTargets;

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	
	void PerformOverlap();

	UPROPERTY()
	FGameplayAbilityTargetingLocationInfo StartLocation;

	float InteractionScanRadius = 100;
	float InteractionScanRate = 0.100;
	
	bool bShowDebug = false;

	FTimerHandle TimerHandle;
};
