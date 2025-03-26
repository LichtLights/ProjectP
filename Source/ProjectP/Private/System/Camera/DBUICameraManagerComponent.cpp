// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Camera/DBUICameraManagerComponent.h"

#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "System/Camera/DBPlayerCameraManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBUICameraManagerComponent)

class AActor;
class FDebugDisplayInfo;

UDBUICameraManagerComponent* UDBUICameraManagerComponent::GetComponent(APlayerController* PC)
{
	if (PC != nullptr)
	{
		if (ADBPlayerCameraManager* PCCamera = Cast<ADBPlayerCameraManager>(PC->PlayerCameraManager))
		{
			return PCCamera->GetUICameraComponent();
		}
	}

	return nullptr;
}

UDBUICameraManagerComponent::UDBUICameraManagerComponent()
{
	bWantsInitializeComponent = true;

	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		// Register "showdebug" hook.
		if (!IsRunningDedicatedServer())
		{
			AHUD::OnShowDebugInfo.AddUObject(this, &ThisClass::OnShowDebugInfo);
		}
	}
}

void UDBUICameraManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UDBUICameraManagerComponent::SetViewTarget(AActor* InViewTarget, FViewTargetTransitionParams TransitionParams)
{
	TGuardValue<bool> UpdatingViewTargetGuard(bUpdatingViewTarget, true);

	ViewTarget = InViewTarget;
	CastChecked<ADBPlayerCameraManager>(GetOwner())->SetViewTarget(ViewTarget, TransitionParams);
}

bool UDBUICameraManagerComponent::NeedsToUpdateViewTarget() const
{
	return false;
}

void UDBUICameraManagerComponent::UpdateViewTarget(struct FTViewTarget& OutVT, float DeltaTime)
{
}

void UDBUICameraManagerComponent::OnShowDebugInfo(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo,
	float& YL, float& YPos)
{
}
