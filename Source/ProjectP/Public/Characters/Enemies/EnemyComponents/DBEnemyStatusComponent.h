// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Components/DBStatusComponent.h"
#include "DBEnemyStatusComponent.generated.h"


UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class PROJECTP_API UDBEnemyStatusComponent : public UDBStatusComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDBEnemyStatusComponent(const FObjectInitializer& ObjectInitializer);
	
};
