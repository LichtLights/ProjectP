// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/BaseEnemyAIController.h"


// Sets default values
ABaseEnemyAIController::ABaseEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(DefaultBehaviorTree);
}

// Called every frame
void ABaseEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

