// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/Components/AC_Inventory.h"
#include "Core/Interfaces/I_Inventory.h"
#include "DBInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTP_API UDBInventoryComponent : public UAC_Inventory
{
	GENERATED_BODY()

public:

	UDBInventoryComponent();
	
};
