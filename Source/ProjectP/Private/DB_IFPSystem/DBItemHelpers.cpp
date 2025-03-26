// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_IFPSystem/DBItemHelpers.h"

#include "Core/Components/AC_Inventory.h"
#include "Core/Data/FL_InventoryFramework.h"
#include "Core/Data/IFP_CoreData.h"
#include "Core/Items/DA_CoreItem.h"
#include "Kismet/KismetMathLibrary.h"

bool UDBItemHelpers::LootItem(FS_InventoryItem Item, UAC_Inventory* ToComponent)
{
	UAC_Inventory* FromComponent = Item.ParentComponent();

	if (!UFL_InventoryFramework::IsItemValid(Item))
	{
		UFL_InventoryFramework::LogIFPMessage(ToComponent, TEXT("Failed to loot item: Item is not valid"), true, true, 5.0f);
		return false;
	}

	FS_InventoryItem TempItemData = Item;

	if (TempItemData.ItemAsset->CanItemStack())
	{
		for (FS_ContainerSettings ToComponentContainerSetting : ToComponent->ContainerSettings)
		{
			if (ToComponent->CheckCompatibility(TempItemData, ToComponentContainerSetting))
			{
				for (FS_InventoryItem ContainerItem : ToComponentContainerSetting.Items)
				{
					// If item can stack, attempt to go through all the ToComponents containers and their items and try to stack with them
					if (UFL_InventoryFramework::CanStackItems(TempItemData, ContainerItem))
					{
						int32 RemainingCount_TempItem;
						int32 NewStackCount_ContainerItem;
						
						ToComponent->StackTwoItems
						(
							TempItemData,
							ContainerItem,
							RemainingCount_TempItem,
							NewStackCount_ContainerItem
						);

						if (RemainingCount_TempItem <= 0)
						{
							return true;
						}
					}
				}
			}
		}
	}

	if (TempItemData.Count <= 0)
	{
		return true;
	}
	
	// Start looping over all compatible containers
	TArray<FS_ContainerSettings> ContainersFilter;
	auto CompatibleContainers = ToComponent->GetCompatibleContainersForItem(TempItemData, ContainersFilter);

	if (!CompatibleContainers.IsValidIndex(0))
	{
		UFL_InventoryFramework::LogIFPMessage(ToComponent, TEXT("Failed to loot item: Could not find compatible container"), true, true, 5.0f);
		return false;
	}

	for (auto CompatibleContainer : CompatibleContainers)
	{
		bool bSpotFound = false;
		int32 AvailableTile;
		TEnumAsByte<ERotation> NeededRotation;
		ToComponent->GetFirstAvailableTile
		(
			TempItemData,
			CompatibleContainer,
			ToComponent->GetGenericIndexesToIgnore(CompatibleContainer),
			bSpotFound,
			AvailableTile,
			NeededRotation
		);

		if (bSpotFound)
		{
			TempItemData.Rotation = NeededRotation;

			ToComponent->MoveItem
			(
				TempItemData,
				FromComponent,
				ToComponent,
				CompatibleContainer.ContainerIndex,
				AvailableTile,
				TempItemData.Count,
				true,
				true,
				true,
				NeededRotation
			);

			return true;
		}
	}
	
	UFL_InventoryFramework::LogIFPMessage(ToComponent, TEXT("Failed to loot item: Could not find a free spot"), true, true, 5.0f);
	return false;

	// UFL_InventoryFramework::LogIFPMessage(ToComponent, TEXT("Failed to loot item: Could not loot item, undefined issue found"), true, true, 5.0f);
	// return false;
}

void UDBItemHelpers::SellItem(FS_InventoryItem ItemToSell, UAC_Inventory* PlayerComponent, UAC_Inventory* BuyerComponent,
	UAC_Inventory* SellerComponent)
{
	if (ItemToSell.ParentComponent() == SellerComponent)
	{
		UIDA_Currency* Currency;
		if (BuyerComponent->CanAffordItem(ItemToSell, Currency))
		{
			TArray<int32> ContainersToIgnore;
			bool bSpotFound;
			FS_ContainerSettings AvailableContainers;
			int32 AvailableTile;
			TEnumAsByte<ERotation> NeededRotation;
			BuyerComponent->GetFirstAvailableContainerAndTile
			(
				ItemToSell,
				ContainersToIgnore,
				bSpotFound,
				AvailableContainers,
				AvailableTile,
				NeededRotation
			);

			if (bSpotFound)
			{
				auto Price = UFL_InventoryFramework::GetItemPrice(ItemToSell);
				auto Amount = UKismetMathLibrary::FTrunc(Price);
				PlayerComponent->NotifyItemSold
				(
					ItemToSell,
					Currency,
					Amount,
					BuyerComponent,
					SellerComponent
				);

				PlayerComponent->MoveItem
				(
					ItemToSell,
					SellerComponent,
					BuyerComponent,
					AvailableContainers.ContainerIndex,
					AvailableTile,
					-1,
					true,
					false,
					true,
					NeededRotation
				);
			}
		}
	}
}
