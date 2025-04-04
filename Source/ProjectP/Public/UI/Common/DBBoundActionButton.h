﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Input/CommonBoundActionButton.h"

#include "DBBoundActionButton.generated.h"

class UCommonButtonStyle;
class UObject;

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class PROJECTP_API UDBBoundActionButton : public UCommonBoundActionButton
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	void HandleInputMethodChanged(ECommonInputType NewInputMethod);

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> KeyboardStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> GamepadStyle;

	UPROPERTY(EditAnywhere, Category = "Styles")
	TSubclassOf<UCommonButtonStyle> TouchStyle;
};
