// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API AProjectPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AProjectPGameModeBase();

protected:

	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPostProcessComponent* PostProcessComp;

	
	
};
