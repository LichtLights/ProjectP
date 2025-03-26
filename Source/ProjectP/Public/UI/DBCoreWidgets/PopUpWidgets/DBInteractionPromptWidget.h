// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/DBGameplayAbility_Interact.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionOptions.h"
#include "Blueprint/UserWidget.h"
#include "UI/IndicatorSystem/IActorIndicatorWidget.h"
#include "DBInteractionPromptWidget.generated.h"

class UDBGameplayAbility_Interact;

/**
 * 
 */
UCLASS()
class PROJECTP_API UDBInteractionPromptWidget : public UUserWidget, public IIndicatorWidgetInterface
{
	GENERATED_BODY()

public:

	UDBInteractionPromptWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void InitializePromptWidget(UDBGameplayAbility_Interact* InParentInteractAbility);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDBGameplayAbility_Interact> ParentInteractAbility;
	void SetParentInteractAbility(UDBGameplayAbility_Interact* InParentInteractAbility) { ParentInteractAbility = InParentInteractAbility; }

	UFUNCTION(BlueprintCallable)
	TArray<FInteractionOption> GetCurrentTargetOptions() const
	{
		return ParentInteractAbility->GetCurrentTargetOptions();
	}

	UFUNCTION(BlueprintCallable)
	FInteractionOption GetActiveOption() const
	{
		return ParentInteractAbility->GetActiveOption();
	}

	UFUNCTION(BlueprintCallable)
	bool IsTargetInteractable() const;

	UFUNCTION(BlueprintNativeEvent)
	void AppendInteractionOptions();
	void AppendInteractionOptions_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateActiveOption();
	void UpdateActiveOption_Implementation();

	virtual void BindIndicator_Implementation(UIndicatorDescriptor* Indicator) override;
	virtual void UnbindIndicator_Implementation(const UIndicatorDescriptor* Indicator) override;

protected:

	
	
};
