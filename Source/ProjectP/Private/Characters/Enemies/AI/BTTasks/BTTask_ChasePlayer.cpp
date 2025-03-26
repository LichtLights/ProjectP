// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/BTTasks/BTTask_ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase Player Character");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// TODO: refactor.
	const FName BBValue_PlayerLocation = "PlayerLocation";
	FVector PlayerLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBValue_PlayerLocation);

	if (IsValid(OwnerComp.GetAIOwner()))
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (IsValid(AIController))
		{
			AIController->MoveToLocation(PlayerLocation);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AIController is not valid at UBTTask_ChasePlayer::ExecuteTask()"));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OwnerComp.GetAIOwner() is not valid at UBTTask_ChasePlayer::ExecuteTask()"));
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Succeeded;
}
