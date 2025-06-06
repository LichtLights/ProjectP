// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSettingAction.h"
#include "GameSettingValueScalarDynamic.h"

#include "DBSettingAction_SafeZoneEditor.generated.h"

class UGameSetting;
class UObject;

UCLASS()
class UDBSettingValueScalarDynamic_SafeZoneValue : public UGameSettingValueScalarDynamic
{
	GENERATED_BODY()

public:
	
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;
};

/**
 * 
 */
UCLASS()
class UDBSettingAction_SafeZoneEditor : public UGameSettingAction
{
	GENERATED_BODY()
	
public:
	
	UDBSettingAction_SafeZoneEditor();
	virtual TArray<UGameSetting*> GetChildSettings() override;

private:
	UPROPERTY()
	TObjectPtr<UDBSettingValueScalarDynamic_SafeZoneValue> SafeZoneValueSetting;
};
