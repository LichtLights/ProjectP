// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/Doodads/BaseDoodadActor.h"


// Sets default values
ABaseDoodadActor::ABaseDoodadActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseDoodadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseDoodadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

