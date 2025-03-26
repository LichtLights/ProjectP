// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "IndicatorLibrary.generated.h"

class AController;
class UDBIndicatorManagerComponent;
class UObject;
struct FFrame;

UCLASS()
class PROJECTP_API UIndicatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UIndicatorLibrary();
	
	/**  */
	UFUNCTION(BlueprintCallable, Category = Indicator)
	static UDBIndicatorManagerComponent* GetIndicatorManagerComponent(AController* Controller);
};
