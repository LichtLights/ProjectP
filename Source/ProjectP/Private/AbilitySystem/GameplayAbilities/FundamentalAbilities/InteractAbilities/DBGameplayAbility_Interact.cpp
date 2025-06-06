// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/DBGameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionStatics.h"
#include "UI/IndicatorSystem/DBIndicatorManagerComponent.h"
#include "UI/IndicatorSystem/IndicatorDescriptor.h"
#include "NativeGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/Tasks/AbilityTask_ScanForInteractableTargets.h"
#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Physics/DBCollisionChannels.h"
#include "System/PlayerController/DBPlayerController.h"
#include "UI/DBCoreWidgets/PopUpWidgets/DBInteractionPromptWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameplayAbility_Interact)

UDBGameplayAbility_Interact::UDBGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EDBAbilityActivationPolicy::OnSpawn;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UDBGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Start scanning for interactable targets.
	if (AActor* AvatarActor = GetAvatarActorFromActorInfo())
	{
		FGameplayAbilityTargetingLocationInfo StartLocation;
		StartLocation.SourceActor = AvatarActor;
		ScanTask = UAbilityTask_ScanForInteractableTargets::ScanForInteractableTargets(this, StartLocation, InteractionScanRadius, InteractionScanRate, true);
		if (ScanTask)
		{
			ScanTask->OnScanForTargets.AddDynamic(this, &UDBGameplayAbility_Interact::UpdateInteractionsFromScan);
			ScanTask->ReadyForActivation();
		}
	}

	// Listen for a cycle target event.
	if (UAbilityTask_WaitGameplayEvent* CycleTargetTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DBGameplayTags::Ability_Interaction_CycleTarget))
	{
		CycleTargetTask->EventReceived.AddDynamic(this, &UDBGameplayAbility_Interact::OnCycleTargetEvent);
		CycleTargetTask->ReadyForActivation();
	}

	// Listen for a cycle option event.
	if (UAbilityTask_WaitGameplayEvent* CycleOptionTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, DBGameplayTags::Ability_Interaction_CycleOption))
	{
		CycleOptionTask->EventReceived.AddDynamic(this, &UDBGameplayAbility_Interact::OnCycleOptionEvent);
		CycleOptionTask->ReadyForActivation();
	}
}

void UDBGameplayAbility_Interact::CycleTarget()
{
	if (AvailableInteractableTargets.Num() == 0)
	{
		return;
	}
	
	CurrentTargetIndex = (CurrentTargetIndex + 1) % AvailableInteractableTargets.Num();
	ActiveInteractingTarget = AvailableInteractableTargets[CurrentTargetIndex];
	
	CurrentOptionIndex = 0;
	
	UpdateActiveInteractionIndicator();
}

void UDBGameplayAbility_Interact::CycleOption()
{
	if (AvailableInteractableTargets.Num() == 0 || !ActiveInteractingTarget.GetObject())
	{
		return;
	}
	
	// CurrentTargetOptions = GetOptionsForTarget(ActiveInteractingTarget);
	if (CurrentTargetOptions.Num() == 0)
	{
		return;
	}
	CurrentOptionIndex = (CurrentOptionIndex + 1) % CurrentTargetOptions.Num();
	ActiveOption = CurrentTargetOptions[CurrentOptionIndex];
	
	UpdateActiveInteractionIndicator();
}

void UDBGameplayAbility_Interact::OnCycleTargetEvent(FGameplayEventData Payload)
{
	CycleTarget();
}

void UDBGameplayAbility_Interact::OnCycleOptionEvent(FGameplayEventData Payload)
{
	CycleOption();
}

void UDBGameplayAbility_Interact::UpdateInteractionsFromScan(TArray<FInteractionOption>& Options)
{
	bool bResultsChanged = false;

	// When new entries added.
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (!LastScannedOptions.Contains(Options[i]))
		{
			bResultsChanged = true;
			break;
		}
	}
	
	// When former entries removed.
	for (int32 i = 0; i < LastScannedOptions.Num(); ++i)
	{
		if (!Options.Contains(LastScannedOptions[i]))
		{
			bResultsChanged = true;
			break;
		}
	}

	// If nothing has changed, simply return.
	if (!bResultsChanged)
	{
		return;
	}
	
	// Otherwise, store the new results and update interactions.
	LastScannedOptions = Options;
	UpdateInteractions(Options);
}

void UDBGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractionOptions)
{
	// Save the complete list.
	AvailableOptions = InteractionOptions;

	AvailableInteractableTargets.Empty();
	
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystem is not valid at UDBGameplayAbility_Interact::UpdateInteractions()"));
		return;
	}

	for (const FInteractionOption& Option : AvailableOptions)
	{
		// Add available Targets to CurrentInteractableTargets
		TScriptInterface<IInteractableTarget> Target = Option.InteractableTarget;
		if (Target.GetObject() && Target.GetInterface())
		{
			AvailableInteractableTargets.AddUnique(Target);

			// Grant Interaction Specific Ability from Option
			// Check if any of the options need to grant the ability to the user before they can be used.
			if (Option.InteractionAbilityToGrant)
			{
				// Grant the ability to the GAS, otherwise it won't be able to do whatever the interaction is.
				FObjectKey ObjectKey(Option.InteractionAbilityToGrant);
				if (!InteractionAbilityCache.Find(ObjectKey))
				{
					FGameplayAbilitySpec Spec(Option.InteractionAbilityToGrant, 1, INDEX_NONE, this);
					FGameplayAbilitySpecHandle Handle = AbilitySystem->GiveAbility(Spec);
					InteractionAbilityCache.Add(ObjectKey, Handle);
				}
			}
		}
	}
	// TODO: Should remove the ability from the cache when it is no longer needed.

	SortInteractableTargetsByDistance();

	// Reset selection indices.
	CurrentTargetIndex = 0;
	CurrentOptionIndex = 0;
	ActiveOption = FInteractionOption();
	
	if (AvailableInteractableTargets.Num() > 0)
	{
		ActiveInteractingTarget = AvailableInteractableTargets[CurrentTargetIndex];
	}
	else
	{
		ActiveInteractingTarget = TScriptInterface<IInteractableTarget>();
	}

	UpdateActiveInteractionIndicator();
}

void UDBGameplayAbility_Interact::UpdateActiveInteractionIndicator()
{
	// Remove any previously displayed indicators related to this ability.
    if (ADBPlayerController* PC = GetDBPlayerControllerFromActorInfo())
    {
        if (UDBIndicatorManagerComponent* IndicatorManager = UDBIndicatorManagerComponent::GetComponent(PC))
        {
            for (UIndicatorDescriptor* Indicator : Indicators)
            {
                IndicatorManager->RemoveIndicator(Indicator);
            }
        }
    }
	InteractionPromptWidget = nullptr;
    Indicators.Reset();

    // If no targets are available, nothing to show.
    if (AvailableInteractableTargets.Num() == 0)
    {
    	// TODO: DEBUG LOG
	    // UE_LOG(LogTemp, Warning, TEXT("AvailableInteractableTargets is empty at UDBGameplayAbility_Interact::UpdateActiveInteractionIndicator()"));
        return;
    }

	/** Target Index */
	// Reset CurrentTargetIndex if it is out of bounds
    if (CurrentTargetIndex < 0 || CurrentTargetIndex >= AvailableInteractableTargets.Num())
    {
        CurrentTargetIndex = 0;
    }

	// Get the active target and its options.
	CurrentTargetOptions = GetOptionsForTarget(ActiveInteractingTarget);
    if (CurrentTargetOptions.Num() == 0)
    {
		// TODO: DEBUG LOG
	    // UE_LOG(LogTemp, Warning, TEXT("CurrentTargetOptions is empty at UDBGameplayAbility_Interact::UpdateActiveInteractionIndicator()"));
        return;
    }

	/** Option Index */
	// Reset CurrentOptionIndex if it is out of bounds
    if (CurrentOptionIndex < 0 || CurrentOptionIndex >= CurrentTargetOptions.Num())
    {
        CurrentOptionIndex = 0;
    }
	// ActiveOption = FInteractionOption();
    ActiveOption = CurrentTargetOptions[CurrentOptionIndex];

    // Create and add a single indicator for the active option.
    if (ADBPlayerController* PC = GetDBPlayerControllerFromActorInfo())
    {
        if (UDBIndicatorManagerComponent* IndicatorManager = UDBIndicatorManagerComponent::GetComponent(PC))
        {
            AActor* InteractableTargetActor = Cast<AActor>(ActiveInteractingTarget.GetObject());
        	
            TSoftClassPtr<UUserWidget> InteractionWidgetClass = InteractionPromptWidgetClass;
        	// Cast<UDBInteractionPromptWidget>(InteractionWidgetClass);

            UIndicatorDescriptor* Indicator = NewObject<UIndicatorDescriptor>(this);
            Indicator->SetDataObject(InteractableTargetActor);
            if (InteractableTargetActor)
            {
            	// TODO: Set proper SceneComponent via SetSceneComponent? Currently enough for using SetProjectionMode.
                Indicator->SetSceneComponent(InteractableTargetActor->GetRootComponent());
            }
            Indicator->SetIndicatorClass(InteractionWidgetClass);
        	
        	Indicator->SetProjectionMode(EActorCanvasProjectionMode::ComponentBoundingBox);
        	Indicator->SetBoundingBoxAnchor(FVector(0.5f, 0.5f, 1.0f));
        	Indicator->SetVAlign(EVerticalAlignment::VAlign_Bottom);
        	Indicator->SetAutoRemoveWhenIndicatorComponentIsNull(true);
        	Indicator->SetClampToScreen(true);
        	
            IndicatorManager->AddIndicator(Indicator);
            Indicators.Add(Indicator);

        	InteractionPromptWidget = Indicator->IndicatorWidget; // Assign the indicator widget
        	if (UDBInteractionPromptWidget* PromptWidget = Cast<UDBInteractionPromptWidget>(InteractionPromptWidget))
        	{
        		PromptWidget->InitializePromptWidget(this);
        		PromptWidget->UpdateActiveOption();
        	}
        	else
        	{
        		UE_LOG(LogTemp, Warning, TEXT("InteractionPromptWidget is null in UpdateActiveInteractionIndicator()"));
        	}
        }
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't get PlayerController at UDBGameplayAbility_Interact::UpdateActiveInteractionIndicator()"));
	}
}

void UDBGameplayAbility_Interact::TriggerInteraction()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (!AbilitySystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilitySystem is null at UDBGameplayAbility_Interact::TriggerInteraction()"));
		return;
	}

	if (!ActiveOption.InteractableTarget || !ActiveOption.InteractableTarget->IsInteractionReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractableTarget is null or not ready at UDBGameplayAbility_Interact::TriggerInteraction()"));
		return;
	}

	
	AActor* Instigator = GetAvatarActorFromActorInfo();
	AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(ActiveOption.InteractableTarget);
	
	FGameplayEventData Payload;
	Payload.Instigator = Instigator;
	Payload.Target = InteractableTargetActor;
	
	// Determine which event tag to use based on the interaction option type.
	FGameplayTag EventTag = ActiveOption.InteractionGameplayTag;

	// Set the tag in the payload for context.
	Payload.EventTag = EventTag;
	
	// Retrieve the current ability's spec handle.
	FGameplayAbilitySpecHandle AbilityHandle = AbilitySystem->FindAbilitySpecFromClass(ActiveOption.InteractionAbilityToGrant)->Handle;

	// Grab the target actor off the payload we're going to use it as the 'avatar' for the interaction, and the
	// source InteractableTarget actor as the owner actor.
	AActor* TargetActor = const_cast<AActor*>(ToRawPtr(Payload.Target));
	
	// The actor info needed for the interaction.
	FGameplayAbilityActorInfo ActorInfo;
	ActorInfo.InitFromActor(InteractableTargetActor, TargetActor, AbilitySystem);
	
	// Now trigger the ability from the gameplay event.
	// This function will make a temporary copy of the payload, override its tag,
	// and attempt to activate any ability bound to that event.
	const bool bSuccess = AbilitySystem->TriggerAbilityFromGameplayEvent(AbilityHandle, &ActorInfo, EventTag, &Payload, *AbilitySystem);
}

void UDBGameplayAbility_Interact::SortInteractableTargetsByDistance()
{
	// Optionally sort targets by distance from the avatar.
	if (AvailableInteractableTargets.Num() > 0)
	{
		AActor* OwnerActor = GetCurrentActorInfo()->AvatarActor.Get();
		if (OwnerActor)
		{
			const FVector OwnerLocation = OwnerActor->GetActorLocation();
			AvailableInteractableTargets.Sort([OwnerLocation](const TScriptInterface<IInteractableTarget>& A, const TScriptInterface<IInteractableTarget>& B) -> bool
			{
				float DistA = FLT_MAX;
				float DistB = FLT_MAX;
				if (A)
				{
					if (AActor* ActorA = Cast<AActor>(A.GetObject()))
					{
						DistA = FVector::DistSquared(OwnerLocation, ActorA->GetActorLocation());
					}
				}
				if (B)
				{
					if (AActor* ActorB = Cast<AActor>(B.GetObject()))
					{
						DistB = FVector::DistSquared(OwnerLocation, ActorB->GetActorLocation());
					}
				}
				return DistA < DistB;
			});
		}
	}
}

TArray<FInteractionOption> UDBGameplayAbility_Interact::GetOptionsForTarget(
	const TScriptInterface<IInteractableTarget>& InTarget)
{
	TArray<FInteractionOption> OutOptions;
	
	if (!IsValid(ActiveInteractingTarget.GetObject()))
	{
		// TODO: Handle when Target is not valid(or no target is available).
		
	}
	
	for (FInteractionOption Option : AvailableOptions)
	{
		if (Option.InteractableTarget.GetObject() == InTarget.GetObject())
		{
			OutOptions.AddUnique(Option);
		}
	}

	FInteractionOption::SortOptionsByAlphabeticalOrder(OutOptions);
	return OutOptions;
}
