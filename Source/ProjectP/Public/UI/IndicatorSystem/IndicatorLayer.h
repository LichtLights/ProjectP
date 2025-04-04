﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Widget.h"

#include "IndicatorLayer.generated.h"

class SActorCanvas;
class SWidget;
class UObject;

/**
 * 
 */
UCLASS()
class UIndicatorLayer : public UWidget
{
	GENERATED_UCLASS_BODY()
	
public:
	/** Default arrow brush to use if UI is clamped to the screen and needs to show an arrow. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Appearance)
	FSlateBrush ArrowBrush;

protected:
	// UWidget interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End UWidget

protected:
	TSharedPtr<SActorCanvas> MyActorCanvas;
};
