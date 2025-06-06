// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/Tasks/AbilityTask_ScanForInteractableTargets.h"

#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/InteractionStatics.h"
#include "Physics/DBCollisionChannels.h"
#include "Engine/OverlapResult.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_ScanForInteractableTargets)

UAbilityTask_ScanForInteractableTargets::UAbilityTask_ScanForInteractableTargets(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UAbilityTask_ScanForInteractableTargets::Activate()
{
	SetWaitingOnAvatar();

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::PerformOverlap, InteractionScanRate, true);
}

UAbilityTask_ScanForInteractableTargets* UAbilityTask_ScanForInteractableTargets::ScanForInteractableTargets(
	UGameplayAbility* OwningAbility,
	FGameplayAbilityTargetingLocationInfo StartLocation, float InteractionScanRadius, float InteractionScanRate,
	bool bShowDebug)
{
	UAbilityTask_ScanForInteractableTargets* MyObj = NewAbilityTask<UAbilityTask_ScanForInteractableTargets>(OwningAbility);
	MyObj->InteractionScanRadius = InteractionScanRadius;
	MyObj->InteractionScanRate = InteractionScanRate;
	MyObj->StartLocation = StartLocation;
	MyObj->bShowDebug = bShowDebug;

	return MyObj;
}

void UAbilityTask_ScanForInteractableTargets::OnDestroy(bool AbilityEnded)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_ScanForInteractableTargets::PerformOverlap()
{
	if (!Ability || !Ability->GetCurrentActorInfo())
	{
		return;
	}

	AActor* AvatarActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
	if (!AvatarActor)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(AvatarActor);
	
	const bool bTraceComplex = false;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UAbilityTask_ScanForInteractableTargets), bTraceComplex);
	Params.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> OverlapResults;
	bool bHadOverlap = World->OverlapMultiByChannel(
		OUT OverlapResults,
		AvatarActor->GetActorLocation(),
		FQuat::Identity,
		DB_TraceChannel_Interaction,
		FCollisionShape::MakeSphere(InteractionScanRadius),
		Params
	);

	TArray<FInteractionOption> FoundOptions;
	FoundOptions.Empty();
	if (bHadOverlap)
	{
		// This helper function should inspect the overlaps and build FInteractionOption entries.
		UInteractionStatics::AppendInteractionOptionsFromOverlapResults(OverlapResults, FoundOptions);

		// Sort Options.
		FInteractionOption::SortOptionsByAlphabeticalOrder(FoundOptions);
	}
	
	// Broadcast the scan results.
	OnScanForTargets.Broadcast(FoundOptions);
	
#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DebugColor = OverlapResults.Num() > 0 ? FColor::Red : FColor::Green;
		if (OverlapResults.Num() > 0)
		{
			DrawDebugSphere(World, AvatarActor->GetActorLocation(), InteractionScanRadius, 16, DebugColor, false, InteractionScanRate);
		}
		else
		{
			DrawDebugSphere(World, AvatarActor->GetActorLocation(), InteractionScanRadius, 16, DebugColor, false, InteractionScanRate);
		}
	}
#endif // ENABLE_DRAW_DEBUG
}
