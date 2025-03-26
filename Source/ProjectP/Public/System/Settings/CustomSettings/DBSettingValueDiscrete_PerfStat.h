// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameSettingValueDiscrete.h"

#include "DBSettingValueDiscrete_PerfStat.generated.h"

enum class EDBDisplayablePerformanceStat : uint8;
enum class EDBStatDisplayMode : uint8;

class UObject;

/**
 * 
 */
UCLASS()
class UDBSettingValueDiscrete_PerfStat : public UGameSettingValueDiscrete
{
	GENERATED_BODY()
	
public:

	UDBSettingValueDiscrete_PerfStat();

	void SetStat(EDBDisplayablePerformanceStat InStat);

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
	
	void AddMode(FText&& Label, EDBStatDisplayMode Mode);
protected:
	
	TArray<FText> Options;
	TArray<EDBStatDisplayMode> DisplayModes;

	EDBDisplayablePerformanceStat StatToDisplay;
	EDBStatDisplayMode InitialMode;
};
