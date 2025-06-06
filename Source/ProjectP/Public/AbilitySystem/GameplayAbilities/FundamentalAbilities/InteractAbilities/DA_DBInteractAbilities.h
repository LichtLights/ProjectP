// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "DA_DBInteractAbilities.generated.h"

class UDBGameplayAbility;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_DBInteractAbilities : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
	TSubclassOf<UDBGameplayAbility> Interact_Collect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
	TSubclassOf<UDBGameplayAbility> Interact_LootBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
	TSubclassOf<UDBGameplayAbility> Interact_UnlockBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
	TSubclassOf<UDBGameplayAbility> Interact_OpenDoor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
	TSubclassOf<UDBGameplayAbility> Interact_CloseDoor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Interact Abilities")
    TSubclassOf<UDBGameplayAbility> Interact_UnlockDoor;
	
};
