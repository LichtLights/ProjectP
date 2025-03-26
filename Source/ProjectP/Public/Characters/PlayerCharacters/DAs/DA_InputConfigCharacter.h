// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_InputConfigCharacter.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_InputConfigCharacter : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Mapping Context")
	class UInputMappingContext* DefaultMappingContext;
	
	// In-game action inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* Aim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* Interact;

};
