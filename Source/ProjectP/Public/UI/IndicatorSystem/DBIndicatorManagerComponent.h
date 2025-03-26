// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ControllerComponent.h"

#include "DBIndicatorManagerComponent.generated.h"

class AController;
class UIndicatorDescriptor;
class UObject;
struct FFrame;

/**
 * @class UDBIndicatorManagerComponent
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTP_API UDBIndicatorManagerComponent : public UControllerComponent
{
	GENERATED_BODY()

public:

	UDBIndicatorManagerComponent(const FObjectInitializer& ObjectInitializer);
	
	static UDBIndicatorManagerComponent* GetComponent(AController* Controller);

	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* IndicatorDescriptor);
	
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* IndicatorDescriptor);

	DECLARE_EVENT_OneParam(ULyraIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Descriptor)
	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

	const TArray<UIndicatorDescriptor*>& GetIndicators() const { return Indicators; }

private:
	UPROPERTY()
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;

};
