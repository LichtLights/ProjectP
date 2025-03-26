// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Settings/Performance/DBPerformanceSettings.h"

#include "Engine/PlatformSettingsManager.h"
#include "Misc/EnumRange.h"
#include "System/Settings/Performance/DBPerformanceStatTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBPerformanceSettings)

//////////////////////////////////////////////////////////////////////

UDBPlatformSpecificRenderingSettings::UDBPlatformSpecificRenderingSettings()
{
	MobileFrameRateLimits.Append({ 20, 30, 45, 60, 90, 120 });
}

const UDBPlatformSpecificRenderingSettings* UDBPlatformSpecificRenderingSettings::Get()
{
	UDBPlatformSpecificRenderingSettings* Result = UPlatformSettingsManager::Get().GetSettingsForPlatform<ThisClass>();
	check(Result);
	return Result;
}

//////////////////////////////////////////////////////////////////////

UDBPerformanceSettings::UDBPerformanceSettings()
{
	PerPlatformSettings.Initialize(UDBPlatformSpecificRenderingSettings::StaticClass());

	CategoryName = TEXT("Game");

	DesktopFrameRateLimits.Append({ 30, 60, 120, 144, 160, 165, 180, 200, 240, 360 });

	// Default to all stats are allowed
	FDBPerformanceStatGroup& StatGroup = UserFacingPerformanceStats.AddDefaulted_GetRef();
	for (EDBDisplayablePerformanceStat PerfStat : TEnumRange<EDBDisplayablePerformanceStat>())
	{
		StatGroup.AllowedStats.Add(PerfStat);
	}
}
