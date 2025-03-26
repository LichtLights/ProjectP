// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PromptOptionBox.generated.h"

UENUM(BlueprintType)
enum class EPromptBoxHighlight : uint8
{
	None = 0,
	Disabled = 1,
	Active = 2
};

/**
 * 
 */
UCLASS()
class PROJECTP_API UPromptOptionBox : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetPromptHighlight(EPromptBoxHighlight Highlight);
	void SetPromptHighlight_Implementation(EPromptBoxHighlight Highlight);
};
