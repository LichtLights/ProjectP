// Fill out your copyright notice in the Description page of Project Settings.


#include "DBCoreActors/LootBoxes/DBLootBox.h"

#include "GlobalConstants.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "DB_IFPSystem/Inventory/Components/DBInventoryComponent.h"
#include "DB_IFPSystem/Items/DA_DBCoreItem.h"
#include "LootTableSystem/Components/AC_LootTable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBLootBox)

ADBLootBox::ADBLootBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);
	
	BoxComp->SetBoxExtent(FVector(16, 16, 16));
	BoxComp->SetCollisionProfileName(TEXT("InteractableDynamic"), true);
	
	LootBoxSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Loot Box Sprite"));
	LootBoxSprite->SetupAttachment(BoxComp);
	LootBoxSprite->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantRight);
	LootBoxSprite->SetCollisionProfileName(TEXT("NoCollision"));
	LootBoxSprite->SetCastShadow(true);
	
	InventoryComponent = CreateDefaultSubobject<UDBInventoryComponent>(TEXT("Inventory Component"));
	LootTableComponent = CreateDefaultSubobject<UAC_LootTable>(TEXT("LootTable Component"));

	// TODO: Check the item is "Key"
	// if (RequiredKey->ItemType == FGameplayTag::RequestGameplayTag(TEXT("Item.Key")));
	if (RequiredKey)
	{
		bIsLocked = true;
	}
}

void ADBLootBox::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	if (IsLocked())
	{
		OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Unlock_Box, this));
	}
	else
	{
		OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Loot_Box, this));
	}
}

void ADBLootBox::CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag,
	FGameplayEventData& InOutEventData)
{
	IInteractableTarget::CustomizeInteractionEventData(InteractionEventTag, InOutEventData);
}

bool ADBLootBox::IsInteractionReady()
{
	return true;
}

void ADBLootBox::Unlock(UAC_Inventory* InstigatorInventory, bool bShouldRemoveKeyItem)
{
	if (!IsValid(InstigatorInventory))
	{
		UE_LOG(LogTemp, Warning, TEXT("InstigatorInventory is not valid at ADBLootBox::Unlock()"));
		return;
	}

	

	
}

void ADBLootBox::OpenLootBox()
{
	
}

void ADBLootBox::CloseLootBox()
{
	
}
