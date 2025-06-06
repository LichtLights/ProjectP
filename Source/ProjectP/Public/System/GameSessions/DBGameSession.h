// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameSession.h"

#include "DBGameSession.generated.h"

class UObject;

UCLASS(Config = Game)
class PROJECTP_API ADBGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	
	ADBGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

};
