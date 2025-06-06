// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseAbility.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Undetermined = 0,
	Passive = 1,
	Active = 2,
};

/**
 * 
 */
UCLASS()
class PROJECTP_API UBaseAbility : public UObject
{
	GENERATED_BODY()

public:
	
	UBaseAbility();

	// Prerequisite abilities (these must be learned before this ability can be unlocked)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TArray<UBaseAbility*> PrerequisiteAbilities;

	// Name of the Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FText AbilityName;

	// Description of the Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	FText AbilityDescription;

	// Type of the Ability (Active/Passive)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	int32 AbilityPointCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability")
	int32 CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	int32 MaxMasterLevel;

	// Function to check if the prerequisites are met
	UFUNCTION(BlueprintCallable, Category = "Ability")
	bool ArePrerequisitesMet(const TArray<UBaseAbility*>& LearnedAbilities) const;
    
	// Function to upgrade the ability
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void UpgradeAbility();

protected:
	virtual void ApplyAbilityEffect();
};
