﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommonGameInstance.h"

#include "DBGameInstance.generated.h"

class ADBPlayerController;
class UObject;
/**
 * 
 */
UCLASS(Config = Game)
class PROJECTP_API UDBGameInstance : public UCommonGameInstance
{
	GENERATED_BODY()

public:

	UDBGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ADBPlayerController* GetPrimaryPlayerController() const;

	// TODO: Online play related.
	// virtual bool CanJoinRequestedSession() const override;
	// virtual void HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext) override;

	// virtual void ReceivedNetworkEncryptionToken(const FString& EncryptionToken, const FOnEncryptionKeyResponse& Delegate) override;
	// virtual void ReceivedNetworkEncryptionAck(const FOnEncryptionKeyResponse& Delegate) override;

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

	void OnPreClientTravelToSession(FString& URL);

	/** A hard-coded encryption key used to try out the encryption code. This is NOT SECURE, do not use this technique in production! */
	TArray<uint8> DebugTestEncryptionKey;
};
