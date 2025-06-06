// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DBPawnData.generated.h"

class UDBCameraMode;
class UDBInputConfig;
class UDBAbilitySet;
class UDBAbilityTagRelationshipMapping;
/**
 *	UDBPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "DB Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class PROJECTP_API UDBPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UDBPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from ADBPawn or ADBCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DB|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DB|Abilities")
	TArray<TObjectPtr<UDBAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DB|Abilities")
	TObjectPtr<UDBAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DB|Input")
	TObjectPtr<UDBInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DB|Camera")
	TSubclassOf<UDBCameraMode> DefaultCameraMode;
	
};
