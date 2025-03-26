// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/DBInputComponent.h"

#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UDBInputComponent::UDBInputComponent(const FObjectInitializer& ObjectInitializer)
{
	// PrimaryComponentTick.bCanEverTick = true;
}

void UDBInputComponent::AddInputMappings(const UDBInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UDBInputComponent::RemoveInputMappings(const UDBInputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UDBInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
