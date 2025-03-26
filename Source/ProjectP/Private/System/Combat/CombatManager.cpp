// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Combat/CombatManager.h"

#include "GlobalConstants.h"
#include "System/Combat/DamageActors/BaseMeleeSlash.h"

TMap<FString, FMeleeSlashData*> UCombatManager::SlashDataCache;
TMap<FString, FProjectileFireData*> UCombatManager::ProjectileFireDataCache;

UCombatManager::UCombatManager()
{
	
}

FMeleeSlashData UCombatManager::GetSlashByTableRowName(const FString& TableRowName)
{
	if (!SlashDataCache.Contains(TableRowName))
	{
		// ItemDataTable directory path should be set in LoadItemDataTable().
		const UDataTable* SlashTable = LoadSlashTable();
		if (!SlashTable)
		{
			UE_LOG(LogTemp, Error, TEXT("Slash Data Table not found!"));
			return FMeleeSlashData();
		}

		FMeleeSlashData* SlashData = SlashTable->FindRow<FMeleeSlashData>(FName(*TableRowName), "");
		if (!SlashData)
		{
			UE_LOG(LogTemp, Error, TEXT("Slash with TableRowName %s not found in slash data table!"), *TableRowName);
			return FMeleeSlashData();
		}

		SlashDataCache.Add(TableRowName, SlashData);
	}

	FMeleeSlashData SlashToSet = **SlashDataCache.Find(TableRowName);

	if (SlashToSet.MeleeSlash)
	{
		return SlashToSet;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SlashToSet is not valid."));
	}

	return FMeleeSlashData();
}

FProjectileFireData UCombatManager::GetProjectileFireByTableRowName(const FString& TableRowName)
{
	if (!ProjectileFireDataCache.Contains(TableRowName))
	{
		// ItemDataTable directory path should be set in LoadItemDataTable().
		const UDataTable* ProjectileFireTable = LoadSlashTable();
		if (!ProjectileFireTable)
		{
			UE_LOG(LogTemp, Error, TEXT("ProjectileFire Data Table not found!"));
			return FProjectileFireData();
		}

		FProjectileFireData* ProjectileFireData = ProjectileFireTable->FindRow<FProjectileFireData>(FName(*TableRowName), "");
		if (!ProjectileFireData)
		{
			UE_LOG(LogTemp, Error, TEXT("ProjectileFire with TableRowName %s not found in slash data table!"), *TableRowName);
			return FProjectileFireData();
		}

		ProjectileFireDataCache.Add(TableRowName, ProjectileFireData);
	}

	FProjectileFireData ProjectileFireToSet = **ProjectileFireDataCache.Find(TableRowName);

	if (ProjectileFireToSet.ProjectileFire)
	{
		return ProjectileFireToSet;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileFireToSet is not valid."));
	}

	return FProjectileFireData();
}

UDataTable* UCombatManager::LoadSlashTable()
{
	// TODO: Directory should be set
	return Cast<UDataTable>(
		StaticLoadObject(UDataTable::StaticClass(),
			nullptr,
			TEXT("/Game/ProjectP/Combat/DamagingActors/DataTables/DT_MeleeSlash.DT_MeleeSlash"))
	);
}

UDataTable* UCombatManager::LoadProjectileFireTable()
{
	// TODO: Directory should be set
	return Cast<UDataTable>(
		StaticLoadObject(UDataTable::StaticClass(),
			nullptr,
			TEXT("/Game/ProjectP/Items/ItemDataTable/DT_ItemData.DT_ItemData"))
	);
}

void UCombatManager::MeleeSlash(UWorld* World, AActor* CasterActor, EMeleeSlashType MeleeSlashType,
                                FVector CasterLocation, FRotator CasterRotation, FVector SpawnLocation, FRotator SpawnRotation)
{
	// Calculate the direction from the caster to the target location
    FVector Direction = (SpawnLocation - CasterLocation).GetSafeNormal();

    FRotator TargetRotation = Direction.Rotation();

    TargetRotation.Pitch = GlobalConstants::SpriteRotatorConstantRight.Pitch;
	
    SpawnRotation = TargetRotation;

    // Ensure the spawn location Z remains fixed
    SpawnLocation.Z = CasterLocation.Z;

	switch (MeleeSlashType)
	{
	case EMeleeSlashType::Test:

		TSubclassOf<ABaseMeleeSlash> SlashClass = GetSlashByTableRowName(TEXT("Test")).MeleeSlash;
		if (!SlashClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("SlashClass is null at UCombatManager::MeleeSlash()"));
			return;
		}

		ABaseMeleeSlash* Slash = World->SpawnActor<ABaseMeleeSlash>(SlashClass, SpawnLocation, SpawnRotation);
		if (Slash)
		{
			UE_LOG(LogTemp, Warning, TEXT("Slash is null at UCombatManager::MeleeSlash()"));
			Slash->ActorsToIgnore.Add(CasterActor);
		}
		
		
		break;
	}
}
