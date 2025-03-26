// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractionOptions.h"
#include "AbilitySystem/GameplayAbilities/DBGameplayAbility.h"

#include "DBGameplayAbility_Interact.generated.h"

class UAbilityTask_ScanForInteractableTargets;
class UDBInteractionPromptWidget;
class UIndicatorDescriptor;

/**
 *	UDBGameplayAbility_Interact
 *
 *	Gameplay ability used for character interacting
 *	This version shows a single interaction prompt widget for the currently selected target/option.
 *	The player can cycle through available targets/options.
 *	It uses an ability task to continuously scan for interactable targets.
 */
UCLASS()
class PROJECTP_API UDBGameplayAbility_Interact : public UDBGameplayAbility
{
	GENERATED_BODY()

public:

	UDBGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	/** Cycle to the next available target. */
	UFUNCTION(BlueprintCallable)
	void CycleTarget();

	/** Cycle to the next option for the current target. */
	UFUNCTION(BlueprintCallable)
	void CycleOption();

	UFUNCTION()
	void OnCycleTargetEvent(FGameplayEventData Payload);
	UFUNCTION()
	void OnCycleOptionEvent(FGameplayEventData Payload);

	TArray<FInteractionOption> GetCurrentTargetOptions() const { return CurrentTargetOptions; }
	FInteractionOption GetActiveOption() const { return ActiveOption; }

	int32 GetCurrentOptionIndex() const { return CurrentOptionIndex; }
	
	/** Delegate callback from the scan task. */
	UFUNCTION()
	void UpdateInteractionsFromScan(TArray<FInteractionOption>& Options);
	
	/** Update available interactions (called by the scan task delegate). */
	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractionOptions);

	/** Groups options by target and updates the active (displayed) prompt widget. */
	void UpdateActiveInteractionIndicator();
	
	/** Trigger the currently selected interaction. */
	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();

	
protected:

	/** All scanned interaction options. */
	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> AvailableOptions;
	
	/** The last scanned interaction options. Used to detect changes. */
	UPROPERTY()
	TArray<FInteractionOption> LastScannedOptions;

	
	/** Distinct interactable targets (keys from OptionsByTarget), sorted by distance. */
	UPROPERTY(BlueprintReadOnly)
	TArray<TScriptInterface<IInteractableTarget>> AvailableInteractableTargets;
	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IInteractableTarget> ActiveInteractingTarget;
	
	void SortInteractableTargetsByDistance();
	
	/** Interaction options for the currently selected target. */
	UPROPERTY(BlueprintReadOnly)
	TArray<FInteractionOption> CurrentTargetOptions;
	UPROPERTY(BlueprintReadOnly)
	FInteractionOption ActiveOption;

	TArray<FInteractionOption> GetOptionsForTarget(const TScriptInterface<IInteractableTarget>& InTarget);
	

	/** Index of the currently selected target. */
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentTargetIndex = 0;
	/** For the current target, index of the chosen option. */
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentOptionIndex = 0;

	
	/** The indicator descriptors for the current interaction prompt. */
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;

	
	/** Scan parameters. */
	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;
	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRadius = 100;

	
	/** Widget class to use for the interaction prompt. */
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> InteractionPromptWidgetClass;
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> InteractionPromptWidget;

	
	/** The scan task (kept alive until ability ends). */
	UPROPERTY()
	UAbilityTask_ScanForInteractableTargets* ScanTask;

private:

	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache;
};
