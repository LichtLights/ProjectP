// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemContextInterface.generated.h"

// UENUM(BlueprintType)
// enum class EItemContextClass : uint8
// {
// 	Undetermined = 0,
// 	PickUp = 1,
// 	Search = 2,
// 	Unlock = 3
// };

USTRUCT(BlueprintType)
struct FItemContextInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Use = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Inspect = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Modding = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Discard = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Equip = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsAvailable_Unequip = false;

	void SetAvailableContexts(const bool bUse, const bool bInspect, const bool bModding, const bool bDiscard, const bool bEquip,
	                              const bool bUnequip)
	{
		bIsAvailable_Use = bUse;
		bIsAvailable_Inspect = bInspect;
		bIsAvailable_Modding = bModding;
		bIsAvailable_Discard = bDiscard;
		bIsAvailable_Equip = bEquip;
		bIsAvailable_Unequip = bUnequip;
	};
	
};

// This class does not need to be modified.
UINTERFACE()
class UItemContextInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTP_API IItemContextInterface
{
	GENERATED_BODY()

// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Implement this function in the interactable objects, returns all available interactions
	virtual FItemContextInfo GetAvailableContexts() = 0;
	
};
