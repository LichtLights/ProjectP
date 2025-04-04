﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"

#include "DBCameraComponent.generated.h"

class UCanvas;
class UDBCameraMode;
class UDBCameraModeStack;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FMinimalViewInfo;
template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UDBCameraMode>, FDBCameraModeDelegate);

/**
 *	UDBCameraComponent
 *
 *	The base camera component class used by this project.
 */
UCLASS()
class PROJECTP_API UDBCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:

	UDBCameraComponent(const FObjectInitializer& ObjectInitializer);

	// Returns the camera component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "DB|Camera")
	static UDBCameraComponent* FindCameraComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UDBCameraComponent>() : nullptr); }

	// Returns the target actor that the camera is looking at.
	virtual AActor* GetTargetActor() const { return GetOwner(); }

	// Delegate used to query for the best camera mode.
	FDBCameraModeDelegate DetermineCameraModeDelegate;

	// Add an offset to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	void AddFieldOfViewOffset(float FovOffset) { FieldOfViewOffset += FovOffset; }

	virtual void DrawDebug(UCanvas* Canvas) const;

	// Gets the tag associated with the top layer and the blend weight of it
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;

protected:

	virtual void OnRegister() override;
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	virtual void UpdateCameraModes();

protected:

	// Stack used to blend the camera modes.
	UPROPERTY()
	TObjectPtr<UDBCameraModeStack> CameraModeStack;

	// Offset applied to the field of view.  The offset is only for one frame, it gets cleared once it is applied.
	float FieldOfViewOffset;
	
};
