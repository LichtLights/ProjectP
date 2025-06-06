// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBCoreWidgets/DEV/DBDebugMenu.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// void UDBDebugMenu::Debug_SpawnItem(FString ItemID)
// {
// 	auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
// 	if (!PlayerCharacter)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null at UDBDebugMenu::Debug_SpawnItem(FString ItemID)"));
// 		return;
// 	}
//
// 	const auto ItemObject = UItemDataManager::CreateItemObjectByTableRowName(ItemID, GetWorld());
// 	if (!ItemObject->IsValidLowLevel())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("ItemObejct is not valid at UDBDebugMenu::Debug_SpawnItem(FString ItemID)"));
// 	}
//
// 	Cast<UItemObject>(ItemObject)->DropItemOnLevel(GetWorld(), PlayerCharacter->GetActorLocation(), false);
// }
//
// void UDBDebugMenu::Debug_SpawnBurstItem(FString ItemID, int32 NumberOfItems)
// {
// 	auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
// 	if (!PlayerCharacter)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is null at UDBDebugMenu::Debug_SpawnItem(FString ItemID)"));
// 		return;
// 	}
//
// 	for (int32 i = 0; i < NumberOfItems; i++)
// 	{
// 		const auto ItemObject = UItemDataManager::CreateItemObjectByTableRowName(ItemID, GetWorld());
// 		if (!ItemObject->IsValidLowLevel())
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("ItemObejct is not valid at UDBDebugMenu::Debug_SpawnItem(FString ItemID)"));
// 		}
// 		
// 		Cast<UItemObject>(ItemObject)->DropItemOnLevel(GetWorld(), PlayerCharacter->GetActorLocation(), true);
// 	}
// }
