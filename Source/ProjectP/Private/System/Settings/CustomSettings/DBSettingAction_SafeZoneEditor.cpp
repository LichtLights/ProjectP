// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Settings/CustomSettings/DBSettingAction_SafeZoneEditor.h"

#include "DataSource/GameSettingDataSource.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "System/Player/DBLocalPlayer.h"
#include "System/Settings/DBGameSettingRegistry.h"
#include "System/Settings/DBSettingsLocal.h"
#include "Widgets/Layout/SSafeZone.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBSettingAction_SafeZoneEditor)

class UGameSetting;

#define LOCTEXT_NAMESPACE "DBSettings"

void UDBSettingValueScalarDynamic_SafeZoneValue::ResetToDefault()
{
	Super::ResetToDefault();
	SSafeZone::SetGlobalSafeZoneScale(TOptional<float>(DefaultValue.Get(0.0f)));
}

void UDBSettingValueScalarDynamic_SafeZoneValue::RestoreToInitial()
{
	Super::RestoreToInitial();
	SSafeZone::SetGlobalSafeZoneScale(TOptional<float>(InitialValue));
}

UDBSettingAction_SafeZoneEditor::UDBSettingAction_SafeZoneEditor()
{
	SafeZoneValueSetting = NewObject<UDBSettingValueScalarDynamic_SafeZoneValue>();
	SafeZoneValueSetting->SetDevName(TEXT("SafeZoneValue"));
	SafeZoneValueSetting->SetDisplayName(LOCTEXT("SafeZoneValue_Name", "Safe Zone Value"));
	SafeZoneValueSetting->SetDescriptionRichText(LOCTEXT("SafeZoneValue_Description", "The safezone area percentage."));
	SafeZoneValueSetting->SetDefaultValue(0.0f);
	SafeZoneValueSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetSafeZone));
	SafeZoneValueSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetSafeZone));
	SafeZoneValueSetting->SetDisplayFormat([](double SourceValue, double NormalizedValue){ return FText::AsNumber(SourceValue); });
	SafeZoneValueSetting->SetSettingParent(this);
}

TArray<UGameSetting*> UDBSettingAction_SafeZoneEditor::GetChildSettings()
{
	return { SafeZoneValueSetting };
}

#undef LOCTEXT_NAMESPACE
