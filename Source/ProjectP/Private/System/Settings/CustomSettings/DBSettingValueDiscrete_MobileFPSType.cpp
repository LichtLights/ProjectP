// Fill out your copyright notice in the Description page of Project Settings.

#include "System/Settings/CustomSettings/DBSettingValueDiscrete_MobileFPSType.h"

#include "System/Settings/DBSettingsLocal.h"
#include "System/Settings/Performance/DBPerformanceSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBSettingValueDiscrete_MobileFPSType)

#define LOCTEXT_NAMESPACE "DBSettings"

UDBSettingValueDiscrete_MobileFPSType::UDBSettingValueDiscrete_MobileFPSType()
{
}

void UDBSettingValueDiscrete_MobileFPSType::StoreInitial()
{
	InitialValue = GetValue();
}

void UDBSettingValueDiscrete_MobileFPSType::ResetToDefault()
{
	SetValue(GetDefaultFPS(), EGameSettingChangeReason::ResetToDefault);
}

void UDBSettingValueDiscrete_MobileFPSType::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);;
}

void UDBSettingValueDiscrete_MobileFPSType::SetDiscreteOptionByIndex(int32 Index)
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);

	int32 NewMode = FPSOptionsModes.IsValidIndex(Index) ? FPSOptionsModes[Index] : GetDefaultFPS();

	SetValue(NewMode, EGameSettingChangeReason::Change);
}

int32 UDBSettingValueDiscrete_MobileFPSType::GetDiscreteOptionIndex() const
{
	TArray<int32> FPSOptionsModes;
	FPSOptions.GenerateKeyArray(FPSOptionsModes);
	return FPSOptionsModes.IndexOfByKey(GetValue());
}

TArray<FText> UDBSettingValueDiscrete_MobileFPSType::GetDiscreteOptions() const
{
	TArray<FText> Options;
	FPSOptions.GenerateValueArray(Options);

	return Options;
}

void UDBSettingValueDiscrete_MobileFPSType::OnInitialized()
{
	Super::OnInitialized();

	const UDBPlatformSpecificRenderingSettings* PlatformSettings = UDBPlatformSpecificRenderingSettings::Get();
	const UDBSettingsLocal* UserSettings = UDBSettingsLocal::Get();

	for (int32 TestLimit : PlatformSettings->MobileFrameRateLimits)
	{
		if (UDBSettingsLocal::IsSupportedMobileFramePace(TestLimit))
		{
			FPSOptions.Add(TestLimit, MakeLimitString(TestLimit));
		}
	}

	const int32 FirstFrameRateWithQualityLimit = UserSettings->GetFirstFrameRateWithQualityLimit();
	if (FirstFrameRateWithQualityLimit > 0)
	{
		SetWarningRichText(FText::Format(LOCTEXT("MobileFPSType_Note", "<strong>Note: Changing the framerate setting to {0} or higher might lower your Quality Presets.</>"), MakeLimitString(FirstFrameRateWithQualityLimit)));
	}
}

int32 UDBSettingValueDiscrete_MobileFPSType::GetValue() const
{
	return UDBSettingsLocal::Get()->GetDesiredMobileFrameRateLimit();
}

void UDBSettingValueDiscrete_MobileFPSType::SetValue(int32 NewLimitFPS, EGameSettingChangeReason InReason)
{
	UDBSettingsLocal::Get()->SetDesiredMobileFrameRateLimit(NewLimitFPS);

	NotifySettingChanged(InReason);
}

int32 UDBSettingValueDiscrete_MobileFPSType::GetDefaultFPS() const
{
	return UDBSettingsLocal::GetDefaultMobileFrameRate();
}

FText UDBSettingValueDiscrete_MobileFPSType::MakeLimitString(int32 Number)
{
	return FText::Format(LOCTEXT("MobileFrameRateOption", "{0} FPS"), FText::AsNumber(Number));
}

#undef LOCTEXT_NAMESPACE
