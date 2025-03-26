// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_InputConfigController.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class PROJECTP_API UDA_InputConfigController : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Mapping Context")
	class UInputMappingContext* DefaultMappingContext;
	
	// In-game UI interaction inputs.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	const UInputAction* ToggleCharacterMenuAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	const UInputAction* ToggleInventoryAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	const UInputAction* ToggleWeaponMasteryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* RotateItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* WheelUp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* WheelDown;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input Action")
	UInputAction* TabPressed;


	// DEBUG test inputs.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="DEBUG Input Action")
	UInputAction* ToggleDebugMenuAction;
	
};
