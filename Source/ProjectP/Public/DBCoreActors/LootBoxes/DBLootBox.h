// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "GameFramework/Actor.h"
#include "DBLootBox.generated.h"

class UAC_Inventory;
class UAC_LootTable;
class UDA_DBCoreItem;
class UPaperSpriteComponent;
class UBoxComponent;
class UDBInventoryComponent;

UCLASS()
class PROJECTP_API ADBLootBox : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:

	ADBLootBox(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DB|LootBox")
	TSubclassOf<UUserWidget> LootBoxWidgetClass;

	//~IInteractableTarget interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) override;
	virtual bool IsInteractionReady() override;
	//~End of IInteractableTarget interface

	UFUNCTION(BlueprintCallable, Category = "DB|LootBox")
	bool IsLocked() const { return bIsLocked; }

	UFUNCTION(BlueprintCallable, Category = "DB|LootBox")
	void Unlock(UAC_Inventory* InstigatorInventory, bool bShouldRemoveKeyItem = false);

	UFUNCTION(BlueprintCallable, Category = "DB|LootBox")
	void OpenLootBox();

	UFUNCTION(BlueprintCallable, Category = "DB|LootBox")
	void CloseLootBox();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDA_DBCoreItem> RequiredKey;

	bool bIsLocked = false;
	
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPaperSpriteComponent> LootBoxSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDBInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UAC_LootTable> LootTableComponent;

};