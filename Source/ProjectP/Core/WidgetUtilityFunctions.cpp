// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetUtilityFunctions.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

bool UWidgetUtilityFunctions::SetWidgetPositionInsideViewportWithCursorOffset(UUserWidget* TargetWidget,
                                                                              APlayerController* PlayerController,
                                                                              FVector2D Offset)
{
	FVector2D MousePosition;
	if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, MousePosition.X, MousePosition.Y))
	{
		// Get viewport size
		const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(PlayerController);

		// Get the size of the widget
		const FVector2D WidgetSize = TargetWidget->GetDesiredSize();

		// Initial aligned position based on mouse position and offset
		FVector2D AlignedPosition = MousePosition - (WidgetSize * 0.5f);
		FVector2D FinalPosition = AlignedPosition + Offset;

		// Adjust X-axis to prevent going out of bounds
		if (FinalPosition.X < 0.0f) // Check left edge
		{
			FinalPosition.X = AlignedPosition.X + FMath::Abs(Offset.X); // Flip to the right
		}
		else if (FinalPosition.X + WidgetSize.X > ViewportSize.X) // Check right edge
		{
			FinalPosition.X = AlignedPosition.X - WidgetSize.X - FMath::Abs(Offset.X); // Flip to the left
		}

		// Adjust Y-axis to prevent going out of bounds
		if (FinalPosition.Y < 0.0f) // Check top edge
		{
			FinalPosition.Y = AlignedPosition.Y + FMath::Abs(Offset.Y); // Flip downward
		}
		else if (FinalPosition.Y + WidgetSize.Y > ViewportSize.Y) // Check bottom edge
		{
			FinalPosition.Y = AlignedPosition.Y - WidgetSize.Y - FMath::Abs(Offset.Y); // Flip upward
		}

		// Clamp final position to ensure it is fully within the viewport
		FinalPosition.X = FMath::Clamp(FinalPosition.X, 0.0f, ViewportSize.X - WidgetSize.X);
		FinalPosition.Y = FMath::Clamp(FinalPosition.Y, 0.0f, ViewportSize.Y - WidgetSize.Y);

		// Set the calculated position
		TargetWidget->SetPositionInViewport(FinalPosition, false);
		return true;
	}
	else
	{
		return false;
	}
}

void UWidgetUtilityFunctions::SetWidgetPositionAtCenterOfViewport(UUserWidget* TargetWidget,
	APlayerController* PlayerController)
{
	if (!IsValid(TargetWidget) || !IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning, TEXT("SetWidgetPositionAtCenterOfViewport: Invalid TargetWidget or PlayerController."));
		return;
	}

	// Get the viewport size
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(PlayerController);

	// Get the size of the widget
	const FVector2D WidgetSize = TargetWidget->GetDesiredSize();

	// Calculate the center position of the viewport
	const FVector2D CenterPosition = (ViewportSize * 0.5f) - (WidgetSize * 0.5f);

	// Set the widget's position to the center of the viewport
	TargetWidget->SetPositionInViewport(CenterPosition, false);
}
