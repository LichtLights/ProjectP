// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SaveGameManager.generated.h"

class ADBPlayerCharacter;
class ABasePlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTP_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void SaveGameProgress(ADBPlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable, Category = "SaveLoad")
	void LoadGameProgress(ADBPlayerCharacter* PlayerCharacter);
	
};
