// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/DBSettingScreen.h"

#include "Input/CommonUIInputTypes.h"
#include "System/Player/DBLocalPlayer.h"
#include "System/Settings/DBGameSettingRegistry.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBSettingScreen)

class UGameSettingRegistry;

void UDBSettingScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	BackHandle = RegisterUIActionBinding(FBindUIActionArgs(BackInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleBackAction)));
	ApplyHandle = RegisterUIActionBinding(FBindUIActionArgs(ApplyInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleApplyAction)));
	CancelChangesHandle = RegisterUIActionBinding(FBindUIActionArgs(CancelChangesInputActionData, true, FSimpleDelegate::CreateUObject(this, &ThisClass::HandleCancelChangesAction)));
}

UGameSettingRegistry* UDBSettingScreen::CreateRegistry()
{
	UDBGameSettingRegistry* NewRegistry = NewObject<UDBGameSettingRegistry>();

	if (UDBLocalPlayer* LocalPlayer = CastChecked<UDBLocalPlayer>(GetOwningLocalPlayer()))
	{
		NewRegistry->Initialize(LocalPlayer);
	}

	return NewRegistry;
}

void UDBSettingScreen::HandleBackAction()
{
	if (AttemptToPopNavigation())
	{
		return;
	}

	ApplyChanges();

	DeactivateWidget();
}

void UDBSettingScreen::HandleApplyAction()
{
	ApplyChanges();
}

void UDBSettingScreen::HandleCancelChangesAction()
{
	CancelChanges();
}

void UDBSettingScreen::OnSettingsDirtyStateChanged_Implementation(bool bSettingsDirty)
{
	if (bSettingsDirty)
	{
		if (!GetActionBindings().Contains(ApplyHandle))
		{
			AddActionBinding(ApplyHandle);
		}
		if (!GetActionBindings().Contains(CancelChangesHandle))
		{
			AddActionBinding(CancelChangesHandle);
		}
	}
	else
	{
		RemoveActionBinding(ApplyHandle);
		RemoveActionBinding(CancelChangesHandle);
	}
}
