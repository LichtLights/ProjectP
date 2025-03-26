// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_IFPSystem/LootBoxes/DBLootBox.h"

#include "GlobalConstants.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "DB_IFPSystem/Inventory/Components/DBInventoryComponent.h"

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
}

void ADBLootBox::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Loot_Box, this));

	// TODO: Implement which kind of "key" item needed.
	if (true)
	{
		OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Unlock_Box, this));
	}
}

void ADBLootBox::CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag,
	FGameplayEventData& InOutEventData)
{
	IInteractableTarget::CustomizeInteractionEventData(InteractionEventTag, InOutEventData);
}

bool ADBLootBox::IsInteractionReady()
{
	return false;
}
