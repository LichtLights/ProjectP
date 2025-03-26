// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonLocalPlayer.h"
#include "Containers/Ticker.h"
#include "GameUIManagerSubsystem.h"

#include "DBUIManagerSubsystem.generated.h"

class UCommonActivatableWidget;
struct FGameplayTag;

class UPrimaryGameLayout;
class FSubsystemCollectionBase;
class UObject;

UCLASS()
class UDBUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()

public:

	UDBUIManagerSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	

private:
	
	bool Tick(float DeltaTime);
	void SyncRootLayoutVisibilityToShowHUD();
	
	FTSTicker::FDelegateHandle TickHandle;
};
