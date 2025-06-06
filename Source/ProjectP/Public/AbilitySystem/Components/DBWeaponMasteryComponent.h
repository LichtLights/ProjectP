// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

#include "DBWeaponMasteryComponent.generated.h"


class UDBWeaponMasteryNode;
class UDBWeaponMasteryTree;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTP_API UDBWeaponMasteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UDBWeaponMasteryComponent(const FObjectInitializer& ObjectInitializer);
	
	//~UActorComponent interface
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Mastery")
	UDBWeaponMasteryTree* MasteryTree;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Mastery")
	TArray<UDBWeaponMasteryNode*> UnlockedAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Mastery")
	int32 AbilityPoints = 0;

	UFUNCTION(BlueprintCallable, Category="Weapon Mastery")
	bool UnlockAbility(UDBWeaponMasteryNode* AbilityNode);

	UFUNCTION(BlueprintCallable, Category="Weapon Mastery")
	bool LevelUpAbility(UDBWeaponMasteryNode* AbilityNode);

	UFUNCTION(BlueprintCallable, Category="Weapon Mastery")
	void GrantUnlockedAbilities();
};
