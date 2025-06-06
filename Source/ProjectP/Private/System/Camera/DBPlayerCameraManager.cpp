// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Camera/DBPlayerCameraManager.h"

#include "Async/TaskGraphInterfaces.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "System/Camera/DBCameraComponent.h"
#include "System/Camera/DBUICameraManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBPlayerCameraManager)

class FDebugDisplayInfo;

static FName UICameraComponentName(TEXT("UICamera"));

ADBPlayerCameraManager::ADBPlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = DB_CAMERA_DEFAULT_FOV;
	ViewPitchMin = DB_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = DB_CAMERA_DEFAULT_PITCH_MAX;

	UICamera = CreateDefaultSubobject<UDBUICameraManagerComponent>(UICameraComponentName);
}

UDBUICameraManagerComponent* ADBPlayerCameraManager::GetUICameraComponent() const
{
	return UICamera;
}

void ADBPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	// If the UI Camera is looking at something, let it have priority.
	if (UICamera->NeedsToUpdateViewTarget())
	{
		Super::UpdateViewTarget(OutVT, DeltaTime);
		UICamera->UpdateViewTarget(OutVT, DeltaTime);
		return;
	}

	Super::UpdateViewTarget(OutVT, DeltaTime);
}

void ADBPlayerCameraManager::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL,
	float& YPos)
{
	check(Canvas);

	FDisplayDebugManager& DisplayDebugManager = Canvas->DisplayDebugManager;

	DisplayDebugManager.SetFont(GEngine->GetSmallFont());
	DisplayDebugManager.SetDrawColor(FColor::Yellow);
	DisplayDebugManager.DrawString(FString::Printf(TEXT("DBPlayerCameraManager: %s"), *GetNameSafe(this)));

	Super::DisplayDebug(Canvas, DebugDisplay, YL, YPos);

	const APawn* Pawn = (PCOwner ? PCOwner->GetPawn() : nullptr);

	if (const UDBCameraComponent* CameraComponent = UDBCameraComponent::FindCameraComponent(Pawn))
	{
		CameraComponent->DrawDebug(Canvas);
	}
}
