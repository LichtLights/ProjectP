// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "BaseDoodadActor.generated.h"

UCLASS()
class PROJECTP_API ABaseDoodadActor : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseDoodadActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
