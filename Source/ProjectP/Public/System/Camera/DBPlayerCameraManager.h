// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/PlayerCameraManager.h"

#include "DBPlayerCameraManager.generated.h"

class FDebugDisplayInfo;
class UCanvas;
class UObject;

#define DB_CAMERA_DEFAULT_FOV			(80.0f)
#define DB_CAMERA_DEFAULT_PITCH_MIN	(-89.0f)
#define DB_CAMERA_DEFAULT_PITCH_MAX	(89.0f)

class UDBUICameraManagerComponent;

/**
 *	ADBPlayerCameraManager
 *
 *	The base player camera manager class used by this project.
 */
UCLASS()
class PROJECTP_API ADBPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	
	ADBPlayerCameraManager(const FObjectInitializer& ObjectInitializer);

	UDBUICameraManagerComponent* GetUICameraComponent() const;

protected:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;

	virtual void DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DebugDisplay, float& YL, float& YPos) override;

private:
	/** The UI Camera Component, controls the camera when UI is doing something important that gameplay doesn't get priority over. */
	UPROPERTY(Transient)
	TObjectPtr<UDBUICameraManagerComponent> UICamera;
};
