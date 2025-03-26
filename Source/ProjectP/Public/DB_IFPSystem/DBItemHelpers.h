// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DBItemHelpers.generated.h"

class UAC_Inventory;
struct FS_InventoryItem;

/**
 * 
 */
UCLASS()
class PROJECTP_API UDBItemHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(Category = "IFP|Items", BlueprintCallable)
	static bool LootItem(FS_InventoryItem Item, UAC_Inventory* ToComponent);

	UFUNCTION(Category = "IFP|Items", BlueprintCallable)
	static void SellItem
	(
		FS_InventoryItem ItemToSell,
		UAC_Inventory* PlayerComponent,
		UAC_Inventory* BuyerComponent,
		UAC_Inventory* SellerComponent
	);
	
};
