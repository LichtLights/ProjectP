// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "DBSettingValueDiscrete_Language.generated.h"

class FText;
class UObject;

/**
 * 
 */
UCLASS()
class UDBSettingValueDiscrete_Language : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UDBSettingValueDiscrete_Language();

	/** UGameSettingValue */
	virtual void StoreInitial() override;
	virtual void ResetToDefault() override;
	virtual void RestoreToInitial() override;

	/** UGameSettingValueDiscrete */
	virtual void SetDiscreteOptionByIndex(int32 Index) override;
	virtual int32 GetDiscreteOptionIndex() const override;
	virtual TArray<FText> GetDiscreteOptions() const override;

protected:
	/** UGameSettingValue */
	virtual void OnInitialized() override;
	virtual void OnApply() override;

protected:
	TArray<FString> AvailableCultureNames;
};
