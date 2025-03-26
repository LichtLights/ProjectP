// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DBActivatableWidget.h"

#include "DBHUDLayout.generated.h"

class UCommonActivatableWidget;
class UObject;

/**
 *	UDBHUDLayout
 *
 *	Widget used to lay out the player's HUD (typically specified by an Add Widgets action in the experience)
 */
UCLASS()
class PROJECTP_API UDBHUDLayout : public UDBActivatableWidget
{
	GENERATED_BODY()

public:

	UDBHUDLayout(const FObjectInitializer& ObjectInitializer);

	void NativeOnInitialized() override;

protected:
	void HandleEscapeAction();

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
	
};
