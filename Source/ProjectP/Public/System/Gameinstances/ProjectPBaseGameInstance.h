// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectPBaseGameInstance.generated.h"

class UItemDataManager;
/**
 * 
 */
UCLASS()
class PROJECTP_API UProjectPBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Post Process")
	class AGlobalPostProcessManager* GlobalPostProcessManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Damage System")
	class AGlobalDamageSystemManager* GlobalDamageSystemManager;

	
	// Saves the player's progress by calling the SaveGameManager
	UFUNCTION(BlueprintCallable, Category="SaveLoad")
	void SavePlayerProgress();
	// Loads the player's progress by calling the SaveGameManager
	UFUNCTION(BlueprintCallable, Category="SaveLoad")
	void LoadPlayerProgress();

	

protected:
	// SaveGameManager to handle the save and load logic
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveLoad")
	class USaveGameManager* SaveGameManager;
};
