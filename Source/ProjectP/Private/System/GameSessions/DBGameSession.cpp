// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameSessions/DBGameSession.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameSession)

ADBGameSession::ADBGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ADBGameSession::ProcessAutoLogin()
{
	// This is actually handled in LyraGameMode::TryDedicatedServerLogin
	return true;
}

void ADBGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ADBGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}
