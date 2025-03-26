// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Core/Interfaces/I_Inventory.h"
#include "Core/Interfaces/I_InventoryWidgets.h"
#include "IFPInventoryPanel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UIFPInventoryPanel : public UCommonUserWidget, public II_InventoryWidgets
{
	GENERATED_BODY()

public:

	UIFPInventoryPanel(const FObjectInitializer& ObjectInitializer);
};
