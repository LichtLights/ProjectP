// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BaseSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FInventorySaveData
{
	GENERATED_BODY()

	// All the variables should be updated when inventory component changed.
	bool IsValid() const;
	

	// Total slots.
	UPROPERTY()
	int32 TotalInventorySize = 0;
	UPROPERTY()
	int32 DefaultInventorySize = 10;
	UPROPERTY()
	int32 ByAbilityAddedInventorySize = 0;
	UPROPERTY()
	int32 ByItemAddedInventorySize = 0;

	int32 FormerInventorySize = 0;
};

USTRUCT()
struct FPlayerStatusSaveData
{
	GENERATED_BODY()

	bool IsValid() const;

	// TODO: Add Status from component when all status set up.

	
	
};

UCLASS()
class PROJECTP_API UBaseSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category="Player Data")
	FInventorySaveData InventorySaveData;

	UPROPERTY(VisibleAnywhere, Category="Player Data")
	FPlayerStatusSaveData StatusSaveData;
	
};
