// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbilities/FundamentalAbilities/InteractAbilities/IInteractableTarget.h"
#include "GameFramework/Actor.h"
#include "DBLootBox.generated.h"

class UPaperSpriteComponent;
class UBoxComponent;
class UDBInventoryComponent;

UCLASS()
class PROJECTP_API ADBLootBox : public AActor, public IInteractableTarget
{
	GENERATED_BODY()

public:

	ADBLootBox(const FObjectInitializer& ObjectInitializer);

	//~IInteractableTarget interface
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual void CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag, FGameplayEventData& InOutEventData) override;
	virtual bool IsInteractionReady() override;
	//~End of IInteractableTarget interface

	
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPaperSpriteComponent> LootBoxSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|LootBox", meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UDBInventoryComponent> InventoryComponent;

};