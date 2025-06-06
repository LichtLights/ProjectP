// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Gameinstances/GlobalDamageSystemManager.h"


// Sets default values
AGlobalDamageSystemManager::AGlobalDamageSystemManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGlobalDamageSystemManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlobalDamageSystemManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

