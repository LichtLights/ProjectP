// Fill out your copyright notice in the Description page of Project Settings.


#include "System/SaveGame/SaveGameManager.h"

#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/SaveGame/BaseSaveGame.h"

void USaveGameManager::SaveGameProgress(ADBPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in USaveGameManager::SaveGameProgress(ABasePlayerCharacter* PlayerCharacter)"));
		return;
	}

	UBaseSaveGame* SaveGameInstance = Cast<UBaseSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		// Extract and set inventory data
		// UInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
		// if (InventoryComponent)
		// {
			// TODO: Depreciated Slot System related.
			// auto InventoryDataToSave = InventoryComponent->GetInventorySaveData();
			// if (InventoryDataToSave.IsValid())
			// {
			// 	SaveGameInstance->InventorySaveData = InventoryDataToSave;
			// }
			// else
			// {
			// 	UE_LOG(LogTemp, Error, TEXT("InventoryDataToSave is null at USaveGameManager::SaveGameProgress"));
			// 	UE_LOG(LogTemp, Error, TEXT("Failed to save the game."));
			// 	return;
			// }
		// }

		// // Extract and set status data
		// UPlayerStatusComponent* StatusComponent = PlayerCharacter->FindComponentByClass<UPlayerStatusComponent>();
		// if (StatusComponent)
		// {
		// 	auto StatusDataToSave = StatusComponent->GetStatusSaveData();
		// 	if (StatusDataToSave.IsValid())
		// 	{
		// 		SaveGameInstance->StatusSaveData = StatusDataToSave;
		// 	}
		// 	else
		// 	{
		// 		UE_LOG(LogTemp, Error, TEXT("StatusDataToSave is null at USaveGameManager::SaveGameProgress"));
		// 		UE_LOG(LogTemp, Error, TEXT("Failed to save the game."));
		// 		return;
		// 	}
		// }

		// Save to slot
		// TODO: Determine SaveSlot.
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("PlayerSaveSlot"), 0);
	}
	
}

void USaveGameManager::LoadGameProgress(ADBPlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null in USaveGameManager::SaveGameProgress(ABasePlayerCharacter* PlayerCharacter)"));
		return;
	}

	UBaseSaveGame* LoadGameInstance = Cast<UBaseSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

	if (LoadGameInstance)
	{
		// // Apply inventory data
		// UInventoryComponent* InventoryComponent = PlayerCharacter->FindComponentByClass<UInventoryComponent>();
		// if (InventoryComponent)
		// {
		// 	if (LoadGameInstance->InventorySaveData.IsValid())
		// 	{
		// 		// TODO: Depreciated Slot System related.
		// 		// InventoryComponent->SetInventoryLoadData(LoadGameInstance->InventorySaveData);
		// 	}
		// 	else
		// 	{
		// 		UE_LOG(LogTemp, Error, TEXT("InventorySaveData is not valid at USaveGameManager::LoadGameProgress"));
		// 		UE_LOG(LogTemp, Error, TEXT("Failed to load the game."));
		// 		return;
		// 	}
		// }

		// // Apply status data
		// UPlayerStatusComponent* StatusComponent = PlayerCharacter->FindComponentByClass<UPlayerStatusComponent>();
		// if (StatusComponent)
		// {
		// 	if (LoadGameInstance->StatusSaveData.IsValid())
		// 	{
		// 		StatusComponent->SetStatusLoadData(LoadGameInstance->StatusSaveData);
		// 	}
		// 	else
		// 	{
		// 		UE_LOG(LogTemp, Error, TEXT("StatusSaveData is not valid at USaveGameManager::LoadGameProgress"));
		// 		UE_LOG(LogTemp, Error, TEXT("Failed to load the game."));
		// 		return;
		// 	}
		// }
	}
}
