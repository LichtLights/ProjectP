// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Gameinstances/ProjectPBaseGameInstance.h"

#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/Gameinstances/GlobalDamageSystemManager.h"
#include "System/Gameinstances/GlobalPostProcessManager.h"
#include "System/SaveGame/SaveGameManager.h"

void UProjectPBaseGameInstance::Init()
{
	Super::Init();

	// Spawn the PostProcessManager actor
	if (GetWorld())
	{
		GlobalPostProcessManager = GetWorld()->SpawnActor<AGlobalPostProcessManager>(AGlobalPostProcessManager::StaticClass());
	}

	// Spawn the DamageSystemManager actor
	if (GetWorld())
	{
		GlobalDamageSystemManager = GetWorld()->SpawnActor<AGlobalDamageSystemManager>(AGlobalDamageSystemManager::StaticClass());
	}

	// Initialize the SaveGameManager
	SaveGameManager = NewObject<USaveGameManager>(this, USaveGameManager::StaticClass());
	if (!SaveGameManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SaveGameManager in UProjectPBaseGameInstance::Init()"));
	}
}

// // Example of setting post-process settings
//
// if (UYourGameInstance* GI = Cast<UYourGameInstance>(GetGameInstance()))
// {
// 	if (GI->PostProcessManager)
// 	{
// 		FPostProcessSettings NewSettings;
// 		// Configure your new settings here
// 		GI->PostProcessManager->SetPostProcessSettings(NewSettings);
// 	}
// }

void UProjectPBaseGameInstance::SavePlayerProgress()
{
	// Get the player character
	ADBPlayerCharacter* PlayerCharacter = Cast<ADBPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter && SaveGameManager)
	{
		SaveGameManager->SaveGameProgress(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter or SaveGameManager is null at UProjectPBaseGameInstance::SavePlayerProgress()"));
	}
}

void UProjectPBaseGameInstance::LoadPlayerProgress()
{
	// Get the player character
	ADBPlayerCharacter* PlayerCharacter = Cast<ADBPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter && SaveGameManager)
	{
		SaveGameManager->LoadGameProgress(PlayerCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter or SaveGameManager is null at UProjectPBaseGameInstance::LoadPlayerProgress()"));
	}
}
