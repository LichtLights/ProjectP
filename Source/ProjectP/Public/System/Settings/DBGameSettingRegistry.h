// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSettingRegistry.h"

#include "DBGameSettingRegistry.generated.h"

class ULocalPlayer;
class UObject;

//--------------------------------------
// UDBGameSettingRegistry
//--------------------------------------

class UGameSettingCollection;
class UDBLocalPlayer;

DECLARE_LOG_CATEGORY_EXTERN(LogDBGameSettingRegistry, Log, Log);

#define GET_SHARED_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UDBLocalPlayer, GetSharedSettings),				\
		GET_FUNCTION_NAME_STRING_CHECKED(UDBSettingsShared, FunctionOrPropertyName)			\
	}))

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(UDBLocalPlayer, GetLocalSettings),					\
		GET_FUNCTION_NAME_STRING_CHECKED(UDBSettingsLocal, FunctionOrPropertyName)			\
	}))

/**
 * 
 */
UCLASS()
class UDBGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:

	UDBGameSettingRegistry();
	
	static UDBGameSettingRegistry* Get(UDBLocalPlayer* InLocalPlayer);
	
	virtual void SaveChanges() override;

protected:
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual bool IsFinishedInitializing() const override;

	UGameSettingCollection* InitializeVideoSettings(UDBLocalPlayer* InLocalPlayer);
	void InitializeVideoSettings_FrameRates(UGameSettingCollection* Screen, UDBLocalPlayer* InLocalPlayer);
	void AddPerformanceStatPage(UGameSettingCollection* Screen, UDBLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeAudioSettings(UDBLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGameplaySettings(UDBLocalPlayer* InLocalPlayer);

	UGameSettingCollection* InitializeMouseAndKeyboardSettings(UDBLocalPlayer* InLocalPlayer);
	UGameSettingCollection* InitializeGamepadSettings(UDBLocalPlayer* InLocalPlayer);

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> VideoSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> AudioSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GameplaySettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> MouseAndKeyboardSettings;

	UPROPERTY()
	TObjectPtr<UGameSettingCollection> GamepadSettings;
};
