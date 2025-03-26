// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Player/DBLocalPlayer.h"

#include "AudioMixerBlueprintLibrary.h"
#include "GameFramework/PlayerController.h"
#include "System/Settings/DBSettingsLocal.h"
#include "System/Settings/DBSettingsShared.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBLocalPlayer)

class UObject;

UDBLocalPlayer::UDBLocalPlayer()
{
}

void UDBLocalPlayer::PostInitProperties()
{
	Super::PostInitProperties();

	if (UDBSettingsLocal* LocalSettings = GetLocalSettings())
	{
		LocalSettings->OnAudioOutputDeviceChanged.AddUObject(this, &UDBLocalPlayer::OnAudioOutputDeviceChanged);
	}
}

void UDBLocalPlayer::SwitchController(class APlayerController* PC)
{
	Super::SwitchController(PC);
	
	OnPlayerControllerChanged(PlayerController);
}

bool UDBLocalPlayer::SpawnPlayActor(const FString& URL, FString& OutError, UWorld* InWorld)
{
	// return Super::SpawnPlayActor(URL, OutError, InWorld);
	
	const bool bResult = Super::SpawnPlayActor(URL, OutError, InWorld);

	OnPlayerControllerChanged(PlayerController);

	return bResult;
}

void UDBLocalPlayer::InitOnlineSession()
{
	OnPlayerControllerChanged(PlayerController);

	Super::InitOnlineSession();
}

// void UDBLocalPlayer::SetGenericTeamId(const FGenericTeamId& NewTeamID)
// {
// 	// Do nothing, we merely observe the team of our associated player controller
// }
//
// FGenericTeamId UDBLocalPlayer::GetGenericTeamId() const
// {
// 	if (IDBTeamAgentInterface* ControllerAsTeamProvider = Cast<IDBTeamAgentInterface>(PlayerController))
// 	{
// 		return ControllerAsTeamProvider->GetGenericTeamId();
// 	}
// 	else
// 	{
// 		return FGenericTeamId::NoTeam;
// 	}
// }
//
// FOnDBTeamIndexChangedDelegate* UDBLocalPlayer::GetOnTeamIndexChangedDelegate()
// {
// 	return &OnTeamChangedDelegate;
// }

UDBSettingsLocal* UDBLocalPlayer::GetLocalSettings() const
{
	return UDBSettingsLocal::Get();
}

UDBSettingsShared* UDBLocalPlayer::GetSharedSettings() const
{
	if (!SharedSettings)
	{
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;
		
		if (bCanLoadBeforeLogin)
		{
			SharedSettings = UDBSettingsShared::LoadOrCreateSettings(this);
		}
		else
		{
			// We need to wait for user login to get the real settings so return temp ones
			SharedSettings = UDBSettingsShared::CreateTemporarySettings(this);
		}
	}

	return SharedSettings;
}

void UDBLocalPlayer::LoadSharedSettingsFromDisk(bool bForceLoad)
{
	FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && SharedSettings && CurrentNetId == NetIdForSharedSettings)
	{
		// Already loaded once, don't reload
		return;
	}

	ensure(UDBSettingsShared::AsyncLoadOrCreateSettings(this, UDBSettingsShared::FOnSettingsLoadedEvent::CreateUObject(this, &UDBLocalPlayer::OnSharedSettingsLoaded)));
}

void UDBLocalPlayer::OnSharedSettingsLoaded(UDBSettingsShared* LoadedOrCreatedSettings)
{
	// The settings are applied before it gets here
	if (ensure(LoadedOrCreatedSettings))
	{
		// This will replace the temporary or previously loaded object which will GC out normally
		SharedSettings = LoadedOrCreatedSettings;

		NetIdForSharedSettings = GetCachedUniqueNetId();
	}
}

void UDBLocalPlayer::OnAudioOutputDeviceChanged(const FString& InAudioOutputDeviceId)
{
	FOnCompletedDeviceSwap DevicesSwappedCallback;
	DevicesSwappedCallback.BindUFunction(this, FName("OnCompletedAudioDeviceSwap"));
	UAudioMixerBlueprintLibrary::SwapAudioOutputDevice(GetWorld(), InAudioOutputDeviceId, DevicesSwappedCallback);
}

void UDBLocalPlayer::OnCompletedAudioDeviceSwap(const FSwapAudioOutputResult& SwapResult)
{
	if (SwapResult.Result == ESwapAudioOutputDeviceResultState::Failure)
	{
	}
}

void UDBLocalPlayer::OnPlayerControllerChanged(APlayerController* NewController)
{
	// // Stop listening for changes from the old controller
	// FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	// if (IDBTeamAgentInterface* ControllerAsTeamProvider = Cast<IDBTeamAgentInterface>(LastBoundPC.Get()))
	// {
	// 	OldTeamID = ControllerAsTeamProvider->GetGenericTeamId();
	// 	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	// }
	//
	// // Grab the current team ID and listen for future changes
	// FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	// if (IDBTeamAgentInterface* ControllerAsTeamProvider = Cast<IDBTeamAgentInterface>(NewController))
	// {
	// 	NewTeamID = ControllerAsTeamProvider->GetGenericTeamId();
	// 	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	// 	LastBoundPC = NewController;
	// }
	//
	// ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
}

// void UDBLocalPlayer::OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
// {
// 	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
// }
