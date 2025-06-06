// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Subsystems/DBLoadingScreenSubsystem.h"

#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBLoadingScreenSubsystem)

class UUserWidget;

//////////////////////////////////////////////////////////////////////
// ULyraLoadingScreenSubsystem

UDBLoadingScreenSubsystem::UDBLoadingScreenSubsystem()
{
}

void UDBLoadingScreenSubsystem::SetLoadingScreenContentWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (LoadingScreenWidgetClass != NewWidgetClass)
	{
		LoadingScreenWidgetClass = NewWidgetClass;

		OnLoadingScreenWidgetChanged.Broadcast(LoadingScreenWidgetClass);
	}
}

TSubclassOf<UUserWidget> UDBLoadingScreenSubsystem::GetLoadingScreenContentWidget() const
{
	return LoadingScreenWidgetClass;
}
