// Fill out your copyright notice in the Description page of Project Settings.

#include "System/Settings/CustomSettings/DBSettingValueDiscrete_PerfStat.h"

#include "CommonUIVisibilitySubsystem.h"
#include "System/Settings/Performance/DBPerformanceStatTypes.h"
#include "System/Settings/DBSettingsLocal.h"
#include "System/Settings/Performance/DBPerformanceSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBSettingValueDiscrete_PerfStat)

class ULocalPlayer;

#define LOCTEXT_NAMESPACE "DBSettings"

//////////////////////////////////////////////////////////////////////

class FGameSettingEditCondition_PerfStatAllowed : public FGameSettingEditCondition
{
public:
	static TSharedRef<FGameSettingEditCondition_PerfStatAllowed> Get(EDBDisplayablePerformanceStat Stat)
	{
		return MakeShared<FGameSettingEditCondition_PerfStatAllowed>(Stat);
	}

	FGameSettingEditCondition_PerfStatAllowed(EDBDisplayablePerformanceStat Stat)
		: AssociatedStat(Stat)
	{
	}

	//~FGameSettingEditCondition interface
	virtual void GatherEditState(const ULocalPlayer* InLocalPlayer, FGameSettingEditableState& InOutEditState) const override
	{
		const FGameplayTagContainer& VisibilityTags = UCommonUIVisibilitySubsystem::GetChecked(InLocalPlayer)->GetVisibilityTags();

		bool bCanShowStat = false;
		for (const FDBPerformanceStatGroup& Group : GetDefault<UDBPerformanceSettings>()->UserFacingPerformanceStats) //@TODO: Move this stuff to per-platform instead of doing vis queries too?
		{
			if (Group.AllowedStats.Contains(AssociatedStat))
			{
				const bool bShowGroup = (Group.VisibilityQuery.IsEmpty() || Group.VisibilityQuery.Matches(VisibilityTags));
				if (bShowGroup)
				{
					bCanShowStat = true;
					break;
				}
			}
		}

		if (!bCanShowStat)
		{
			InOutEditState.Hide(TEXT("Stat is not listed in UDBPerformanceSettings or is suppressed by current platform traits"));
		}
	}
	//~End of FGameSettingEditCondition interface

private:
	EDBDisplayablePerformanceStat AssociatedStat;
};

//////////////////////////////////////////////////////////////////////

UDBSettingValueDiscrete_PerfStat::UDBSettingValueDiscrete_PerfStat()
{
}

void UDBSettingValueDiscrete_PerfStat::SetStat(EDBDisplayablePerformanceStat InStat)
{
	StatToDisplay = InStat;
	SetDevName(FName(*FString::Printf(TEXT("PerfStat_%d"), (int32)StatToDisplay)));
	AddEditCondition(FGameSettingEditCondition_PerfStatAllowed::Get(StatToDisplay));
}

void UDBSettingValueDiscrete_PerfStat::StoreInitial()
{
	const UDBSettingsLocal* Settings = UDBSettingsLocal::Get();
	InitialMode = Settings->GetPerfStatDisplayState(StatToDisplay);
}

void UDBSettingValueDiscrete_PerfStat::ResetToDefault()
{
	UDBSettingsLocal* Settings = UDBSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, EDBStatDisplayMode::Hidden);
	NotifySettingChanged(EGameSettingChangeReason::ResetToDefault);
}

void UDBSettingValueDiscrete_PerfStat::RestoreToInitial()
{
	UDBSettingsLocal* Settings = UDBSettingsLocal::Get();
	Settings->SetPerfStatDisplayState(StatToDisplay, InitialMode);
	NotifySettingChanged(EGameSettingChangeReason::RestoreToInitial);
}

void UDBSettingValueDiscrete_PerfStat::SetDiscreteOptionByIndex(int32 Index)
{
	if (DisplayModes.IsValidIndex(Index))
	{
		const EDBStatDisplayMode DisplayMode = DisplayModes[Index];

		UDBSettingsLocal* Settings = UDBSettingsLocal::Get();
		Settings->SetPerfStatDisplayState(StatToDisplay, DisplayMode);
	}
	NotifySettingChanged(EGameSettingChangeReason::Change);
}

int32 UDBSettingValueDiscrete_PerfStat::GetDiscreteOptionIndex() const
{
	const UDBSettingsLocal* Settings = UDBSettingsLocal::Get();
	return DisplayModes.Find(Settings->GetPerfStatDisplayState(StatToDisplay));
}

TArray<FText> UDBSettingValueDiscrete_PerfStat::GetDiscreteOptions() const
{
	return Options;
}

void UDBSettingValueDiscrete_PerfStat::OnInitialized()
{
	Super::OnInitialized();

	AddMode(LOCTEXT("PerfStatDisplayMode_None", "None"), EDBStatDisplayMode::Hidden);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextOnly", "Text Only"), EDBStatDisplayMode::TextOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_GraphOnly", "Graph Only"), EDBStatDisplayMode::GraphOnly);
	AddMode(LOCTEXT("PerfStatDisplayMode_TextAndGraph", "Text and Graph"), EDBStatDisplayMode::TextAndGraph);
}

void UDBSettingValueDiscrete_PerfStat::AddMode(FText&& Label, EDBStatDisplayMode Mode)
{
	Options.Emplace(MoveTemp(Label));
	DisplayModes.Add(Mode);
}

#undef LOCTEXT_NAMESPACE
