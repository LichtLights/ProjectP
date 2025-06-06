// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IndicatorSystem/IndicatorLibrary.h"

#include "UI/IndicatorSystem/DBIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorLibrary)

class AController;

UIndicatorLibrary::UIndicatorLibrary()
{
}

UDBIndicatorManagerComponent* UIndicatorLibrary::GetIndicatorManagerComponent(AController* Controller)
{
	return UDBIndicatorManagerComponent::GetComponent(Controller);
}

