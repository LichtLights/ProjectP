// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Interfaces/DamageHandleInterface.h"
#include "UObject/Object.h"
#include "CombatManager.generated.h"

class ABaseProjectileFire;
class ABaseMeleeSlash;
class UDA_DamageActors;

UENUM(BlueprintType)
enum class EDamageWeightLevel : uint8
{
	Light,
	Normal,
	Heavy,
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Magical,
	Elemental_Fire,
	Elemental_Frost,
	Elemental_Electric,
};

UENUM()
enum class EMeleeSlashType : uint8
{
	Test = 0,
};

UENUM()
enum class EProjectileFireType : uint8
{
	Test = 0,
};

USTRUCT(BlueprintType)
struct FMeleeSlashData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABaseMeleeSlash> MeleeSlash;
	
};

USTRUCT()
struct FProjectileFireData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	ABaseProjectileFire* ProjectileFire;
	
};

/**
 * 
 */
UCLASS()
class PROJECTP_API UCombatManager : public UObject, public IDamageHandleInterface
{
	GENERATED_BODY()

public:

	UCombatManager();

	UFUNCTION(BlueprintCallable)
	static FMeleeSlashData GetSlashByTableRowName(const FString& TableRowName);

	UFUNCTION(BlueprintCallable)
	static FProjectileFireData GetProjectileFireByTableRowName(const FString& TableRowName);

	
private:
	
	static UDataTable* LoadSlashTable();
	static UDataTable* LoadProjectileFireTable();

	static TMap<FString, FMeleeSlashData*> SlashDataCache;
	static TMap<FString, FProjectileFireData*> ProjectileFireDataCache;

	
public:
	
	// TODO: Damaging parameter should be added as development progresses.
	UFUNCTION(BlueprintCallable)
	static void MeleeSlash(UWorld* World, AActor* CasterActor, EMeleeSlashType MeleeSlashType,
	                       FVector CasterLocation, FRotator CasterRotation, FVector SpawnLocation, FRotator SpawnRotation);
};
