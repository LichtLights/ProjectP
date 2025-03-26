// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonUserWidget.h"
#include "Core/Interfaces/I_InventoryWidgets.h"
#include "UI/DBCoreWidgets/DBGameMenuPanel.h"

#include "DBInventoryPanel.generated.h"

/**
 *	UDBInventoryPanel
 *
 *	UCommonUserWidget which used as InGameMenu Panel.
 *	Is parent widget panel holds other widgets related to Inventory.
 */
UCLASS()
class PROJECTP_API UDBInventoryPanel : public UDBGameMenuPanel, public II_InventoryWidgets
{
	GENERATED_BODY()

public:

	UDBInventoryPanel(const FObjectInitializer& ObjectInitializer);

};
