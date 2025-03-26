// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBCoreWidgets/PopUpWidgets/DBInteractionPromptWidget.h"

#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/DBGameplayAbility_Interact.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBInteractionPromptWidget)

UDBInteractionPromptWidget::UDBInteractionPromptWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDBInteractionPromptWidget::InitializePromptWidget(UDBGameplayAbility_Interact* InParentInteractAbility)
{
	if (IsValid(InParentInteractAbility))
	{
		SetParentInteractAbility(InParentInteractAbility);
		AppendInteractionOptions();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InParentInteractAbility is not valid at UDBInteractionPromptWidget::InitializePromptWidget()"));
	}
}

bool UDBInteractionPromptWidget::IsTargetInteractable() const
{
	return GetActiveOption().InteractableTarget->IsInteractionReady();
}

void UDBInteractionPromptWidget::AppendInteractionOptions_Implementation()
{
}

void UDBInteractionPromptWidget::UpdateActiveOption_Implementation()
{
}

void UDBInteractionPromptWidget::BindIndicator_Implementation(UIndicatorDescriptor* Indicator)
{
	IIndicatorWidgetInterface::BindIndicator_Implementation(Indicator);

}

void UDBInteractionPromptWidget::UnbindIndicator_Implementation(const UIndicatorDescriptor* Indicator)
{
	IIndicatorWidgetInterface::UnbindIndicator_Implementation(Indicator);
}
