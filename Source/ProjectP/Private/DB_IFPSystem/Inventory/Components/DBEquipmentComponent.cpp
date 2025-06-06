// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_IFPSystem/Inventory/Components/DBEquipmentComponent.h"

#include "Core/Components/AC_Inventory.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBEquipmentComponent)

UDBEquipmentComponent::UDBEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UDBEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory = Cast<UAC_Inventory>(GetOwner()->GetComponentByClass(UAC_Inventory::StaticClass()));
}

void UDBEquipmentComponent::BindDefaultDelegates_Implementation()
{
	Inventory->ItemEquipped.AddDynamic(this, &UDBEquipmentComponent::OnItemEquipped);
	Inventory->ItemUnequipped.AddDynamic(this, &UDBEquipmentComponent::OnItemUnequipped);
}

TArray<FS_InventoryItem> UDBEquipmentComponent::GetAllEquippedItems() const
{
	// TODO: Implement.
	
	return TArray<FS_InventoryItem>();
}

FS_InventoryItem UDBEquipmentComponent::GetEquippedItemByTag(FGameplayTag SlotTag) const
{
	// TODO: Implement.
	
	return FS_InventoryItem();
}
