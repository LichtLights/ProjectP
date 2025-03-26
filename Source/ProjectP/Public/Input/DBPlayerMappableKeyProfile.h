// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserSettings/EnhancedInputUserSettings.h"

#include "DBPlayerMappableKeyProfile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTP_API UDBPlayerMappableKeyProfile : public UEnhancedPlayerMappableKeyProfile
{
	GENERATED_BODY()
	
protected:

	//~ Begin UEnhancedPlayerMappableKeyProfile interface
	virtual void EquipProfile() override;
	virtual void UnEquipProfile() override;
	//~ End UEnhancedPlayerMappableKeyProfile interface
};
