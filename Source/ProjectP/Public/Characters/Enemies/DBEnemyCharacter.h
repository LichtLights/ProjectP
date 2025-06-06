// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DBCharacter.h"
#include "DBEnemyCharacter.generated.h"

class UEnemyLootableComponent;
class UEnemyAbilitySystemComponent;
class UDBAbilitySystemComponent;

UCLASS()
class PROJECTP_API ADBEnemyCharacter : public ADBCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEnemyAbilitySystemComponent> AbilitySystemComp;
};
