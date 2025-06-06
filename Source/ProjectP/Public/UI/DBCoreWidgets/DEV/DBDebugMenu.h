// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/DBActivatableWidget.h"

#include "DBDebugMenu.generated.h"

/**
 *	UDBDebugMenu
 *
 *	A customizable DebugMenu widget for easy debugging in runtime.
 */
UCLASS()
class PROJECTP_API UDBDebugMenu : public UDBActivatableWidget
{
	GENERATED_BODY()

public:

	// UFUNCTION(BlueprintCallable)
	// void Debug_SpawnItem(FString ItemID);
	//
	// UFUNCTION(BlueprintCallable)
	// void Debug_SpawnBurstItem(FString ItemID, int32 NumberOfItems);
};
