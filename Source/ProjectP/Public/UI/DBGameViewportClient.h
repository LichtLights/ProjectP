﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonGameViewportClient.h"

#include "DBGameViewportClient.generated.h"

class UGameInstance;
class UObject;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTP_API UDBGameViewportClient : public UCommonGameViewportClient
{
	GENERATED_BODY()

public:

	UDBGameViewportClient();

	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice = true) override;
};
