// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Settings/DBGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "System/Player/DBLocalPlayer.h"
#include "System/Settings/DBSettingsLocal.h"
#include "System/Settings/DBSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameSettingRegistry)

DEFINE_LOG_CATEGORY(LogDBGameSettingRegistry);

#define LOCTEXT_NAMESPACE "DB"

//--------------------------------------
// UDBGameSettingRegistry
//--------------------------------------

UDBGameSettingRegistry::UDBGameSettingRegistry()
{
}

UDBGameSettingRegistry* UDBGameSettingRegistry::Get(UDBLocalPlayer* InLocalPlayer)
{
	UDBGameSettingRegistry* Registry = FindObject<UDBGameSettingRegistry>(InLocalPlayer, TEXT("DBGameSettingRegistry"), true);
	if (Registry == nullptr)
	{
		Registry = NewObject<UDBGameSettingRegistry>(InLocalPlayer, TEXT("DBGameSettingRegistry"));
		Registry->Initialize(InLocalPlayer);
	}

	return Registry;
}

void UDBGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();
	
	if (UDBLocalPlayer* LocalPlayer = Cast<UDBLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetLocalSettings()->ApplySettings(false);
		
		LocalPlayer->GetSharedSettings()->ApplySettings();
		LocalPlayer->GetSharedSettings()->SaveSettings();
	}
}

void UDBGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	UDBLocalPlayer* DBLocalPlayer = Cast<UDBLocalPlayer>(InLocalPlayer);

	VideoSettings = InitializeVideoSettings(DBLocalPlayer);
	InitializeVideoSettings_FrameRates(VideoSettings, DBLocalPlayer);
	RegisterSetting(VideoSettings);

	AudioSettings = InitializeAudioSettings(DBLocalPlayer);
	RegisterSetting(AudioSettings);

	GameplaySettings = InitializeGameplaySettings(DBLocalPlayer);
	RegisterSetting(GameplaySettings);

	MouseAndKeyboardSettings = InitializeMouseAndKeyboardSettings(DBLocalPlayer);
	RegisterSetting(MouseAndKeyboardSettings);

	GamepadSettings = InitializeGamepadSettings(DBLocalPlayer);
	RegisterSetting(GamepadSettings);
}

bool UDBGameSettingRegistry::IsFinishedInitializing() const
{
	if (Super::IsFinishedInitializing())
	{
		if (UDBLocalPlayer* LocalPlayer = Cast<UDBLocalPlayer>(OwningLocalPlayer))
		{
			if (LocalPlayer->GetSharedSettings() == nullptr)
			{
				return false;
			}
		}

		return true;
	}

	return false;
}
