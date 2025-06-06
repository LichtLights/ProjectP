// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrimaryGameLayout.h"

#include "DBPrimaryGameLayout.generated.h"

/**
 * 
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class PROJECTP_API UDBPrimaryGameLayout : public UPrimaryGameLayout
{
	GENERATED_BODY()

public:
	
	UDBPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

};
