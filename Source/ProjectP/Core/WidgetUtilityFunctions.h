// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetUtilityFunctions.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UWidgetUtilityFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "UI|Widget")
	static bool SetWidgetPositionInsideViewportWithCursorOffset(UUserWidget* TargetWidget,
	                                                            APlayerController* PlayerController,
	                                                            FVector2D Offset = FVector2D(0, 0));

	UFUNCTION(BlueprintCallable, Category="UI|Widget")
	static void SetWidgetPositionAtCenterOfViewport(UUserWidget* TargetWidget, APlayerController* PlayerController);
	
};
