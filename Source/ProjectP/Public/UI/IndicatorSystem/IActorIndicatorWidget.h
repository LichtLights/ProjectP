// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "IActorIndicatorWidget.generated.h"

class AActor;
class UIndicatorDescriptor;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class PROJECTP_API UIndicatorWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IIndicatorWidgetInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void BindIndicator(UIndicatorDescriptor* Indicator);

	UFUNCTION(BlueprintNativeEvent, Category = "Indicator")
	void UnbindIndicator(const UIndicatorDescriptor* Indicator);
};
