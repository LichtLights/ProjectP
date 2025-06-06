// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Data/IFP_CoreData.h"
#include "DBEquipmentComponent.generated.h"

class UAC_Inventory;

UCLASS(Blueprintable, DisplayName = "Equipment Component", ClassGroup=(IFP), meta=(BlueprintSpawnableComponent))
class PROJECTP_API UDBEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UDBEquipmentComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(Category = "Equipment Component", BlueprintReadOnly)
	TObjectPtr<UAC_Inventory> Inventory = nullptr;
	
	virtual void BeginPlay() override;

	/**	Bind any relevant delegates for this component to listen to.
	 *	By default, this will just listen to ItemEquipped and ItemUnequipped
	 *	since that covers vast majority of cases. You might want to override
	 *	this and customize what events this component will listen to.
	 */
	UFUNCTION(Category = "Equipment Component", BlueprintNativeEvent)
	void BindDefaultDelegates();

	
	UFUNCTION(Category = "Equipment Component", BlueprintCallable, BlueprintImplementableEvent)
	void OnItemEquipped(FS_InventoryItem ItemData, const TArray<FName>& Names);
	UFUNCTION(Category = "Equipment Component", BlueprintCallable, BlueprintImplementableEvent)
	void OnItemUnequipped(FS_InventoryItem ItemData, FS_InventoryItem OldItemData, const TArray<FName>& Names);

	UFUNCTION()
	TArray<FS_InventoryItem> GetAllEquippedItems() const;
	UFUNCTION()
	FS_InventoryItem GetEquippedItemByTag(FGameplayTag SlotTag) const;
};
