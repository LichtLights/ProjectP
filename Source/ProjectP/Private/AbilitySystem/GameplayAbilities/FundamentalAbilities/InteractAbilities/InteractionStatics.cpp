﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionStatics.h"

#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionQuery.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/OverlapResult.h"
#include "UObject/ScriptInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InteractionStatics)

UInteractionStatics::UInteractionStatics()
	: Super(FObjectInitializer::Get())
{
}

AActor* UInteractionStatics::GetActorFromInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget)
{
	if (UObject* Object = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(Object))
		{
			return Actor;
		}
		else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
		{
			return ActorComponent->GetOwner();
		}
		else
		{
			unimplemented();
		}
	}

	return nullptr;
}

void UInteractionStatics::GetInteractableTargetsFromActor(AActor* Actor, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	// If the actor is directly interactable, return that.
	TScriptInterface<IInteractableTarget> InteractableActor(Actor);
	if (InteractableActor)
	{
		OutInteractableTargets.Add(InteractableActor);
	}

	// If the actor isn't interactable, it might have a component that has a interactable interface.
	TArray<UActorComponent*> InteractableComponents = Actor ? Actor->GetComponentsByInterface(UInteractableTarget::StaticClass()) : TArray<UActorComponent*>();
	for (UActorComponent* InteractableComponent : InteractableComponents)
	{
		OutInteractableTargets.Add(TScriptInterface<IInteractableTarget>(InteractableComponent));
	}
}

void UInteractionStatics::AppendInteractableTargetsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		TScriptInterface<IInteractableTarget> InteractableActor(Overlap.GetActor());
		if (InteractableActor)
		{
			OutInteractableTargets.AddUnique(InteractableActor);
		}

		TScriptInterface<IInteractableTarget> InteractableComponent(Overlap.GetComponent());
		if (InteractableComponent)
		{
			OutInteractableTargets.AddUnique(InteractableComponent);
		}
	}
}

void UInteractionStatics::AppendInteractableTargetsFromHitResult(const FHitResult& HitResult, TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	TScriptInterface<IInteractableTarget> InteractableActor(HitResult.GetActor());
	if (InteractableActor)
	{
		OutInteractableTargets.AddUnique(InteractableActor);
	}

	TScriptInterface<IInteractableTarget> InteractableComponent(HitResult.GetComponent());
	if (InteractableComponent)
	{
		OutInteractableTargets.AddUnique(InteractableComponent);
	}
}

void UInteractionStatics::AppendInteractionOptionsFromOverlapResults(const TArray<FOverlapResult>& OverlapResults,
	TArray<FInteractionOption>& OutInteractionOptions)
{
	for (const FOverlapResult& Overlap : OverlapResults)
	{
		TScriptInterface<IInteractableTarget> InteractableActor(Overlap.GetActor());
		if (InteractableActor && InteractableActor->IsInteractionReady())
		{
			TArray<FInteractionOption> Options;
			
			FInteractionQuery InteractQuery;
			
			FInteractionOptionBuilder InteractionBuilder(InteractableActor, Options);
			InteractableActor->GatherInteractionOptions(InteractQuery,InteractionBuilder);

			for (auto Option : Options)
			{
				OutInteractionOptions.AddUnique(Option);
			}
			// TODO: Sort Options array by ? here.
		}

		TScriptInterface<IInteractableTarget> InteractableComponent(Overlap.GetComponent());
		if (InteractableComponent && InteractableComponent->IsInteractionReady())
		{
			TArray<FInteractionOption> Options;
			
			FInteractionQuery InteractQuery;
			
			FInteractionOptionBuilder InteractionBuilder(InteractableComponent, Options);
			InteractableComponent->GatherInteractionOptions(InteractQuery,InteractionBuilder);

			for (auto Option : Options)
			{
				OutInteractionOptions.AddUnique(Option);
			}
		}
	}
}
