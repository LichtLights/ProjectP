﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModularGameMode.h"

#include "DBGameMode.generated.h"

class UDBExperienceDefinition;
class UDBPawnData;
/**
 *	Post login event, triggered when a player or bot joins the game as well as after seamless and non seamless travel
 *
 *	This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDBGameModePlayerInitialized, AGameModeBase* /*GameMode*/, AController* /*NewPlayer*/);

/**
 *	ADBGameMode
 *
 *	The base game mode class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base game mode class used by this project."))
class PROJECTP_API ADBGameMode : public AModularGameModeBase
{
	GENERATED_BODY()

public:

	ADBGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DB|Pawn")
	const UDBPawnData* GetPawnDataForController(const AController* InController) const;

	//~AGameModeBase interface
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void InitGameState() override;
	virtual bool UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage) override;
	virtual void GenericPlayerInitialization(AController* NewPlayer) override;
	virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	// Restart (respawn) the specified player or bot next frame
	// - If bForceReset is true, the controller will be reset this frame (abandoning the currently possessed pawn, if any)
	UFUNCTION(BlueprintCallable)
	void RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset = false);

	// Agnostic version of PlayerCanRestart that can be used for both player bots and players
	virtual bool ControllerCanRestart(AController* Controller);

	// Delegate called on player initialization, described above 
	FOnDBGameModePlayerInitialized OnGameModePlayerInitialized;


protected:

	void OnExperienceLoaded(const UDBExperienceDefinition* CurrentExperience);
	bool IsExperienceLoaded() const;

	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource);

	void HandleMatchAssignmentIfNotExpectingOne();

	// TODO: Server play-related functions.
	// bool TryDedicatedServerLogin();
	// void HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode);

	// UFUNCTION()
	// void OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);
};
