// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemInterface.h"
#include "ModularGameState.h"

#include "DBGameState.generated.h"

struct FDBVerbMessage;
class UDBExperienceManagerComponent;
class UDBAbilitySystemComponent;
/**
 *	ADBGameState
 *
 *	The base game state class used by this project.
 */
UCLASS(Config = Game)
class PROJECTP_API ADBGameState : public AModularGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADBGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;
	//~End of AActor interface

	//~AGameStateBase interface
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTransitionCheckpoint(bool bToTransitionMap) override;
	//~End of AGameStateBase interface

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "DB|GameState")
	UDBAbilitySystemComponent* GetDBAbilitySystemComponent() const { return AbilitySystemComponent; }

	// // Send a message that all clients will (probably) get
	// // (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	// UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "DB|GameState")
	// void MulticastMessageToClients(const FDBVerbMessage Message);
	//
	// // Send a message that all clients will be guaranteed to get
	// // (use only for client notifications that cannot handle being lost)
	// UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "DB|GameState")
	// void MulticastReliableMessageToClients(const FDBVerbMessage Message);
	//
	// // Gets the server's FPS, replicated to clients
	// float GetServerFPS() const;

	// // Indicate the local player state is recording a replay
	// void SetRecorderPlayerState(APlayerState* NewPlayerState);
	//
	// // Gets the player state that recorded the replay, if valid
	// APlayerState* GetRecorderPlayerState() const;

	// // Delegate called when the replay player state changes
	// DECLARE_MULTICAST_DELEGATE_OneParam(FOnRecorderPlayerStateChanged, APlayerState*);
	// FOnRecorderPlayerStateChanged OnRecorderPlayerStateChangedEvent;
	

private:
	
	// Handles loading and managing the current gameplay experience
	UPROPERTY()
	TObjectPtr<UDBExperienceManagerComponent> ExperienceManagerComponent;

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "DB|GameState")
	TObjectPtr<UDBAbilitySystemComponent> AbilitySystemComponent;

protected:
	
	// UPROPERTY(Replicated)
	// float ServerFPS;

	// // The player state that recorded a replay, it is used to select the right pawn to follow
	// // This is only set in replay streams and is not replicated normally
	// UPROPERTY(Transient, ReplicatedUsing = OnRep_RecorderPlayerState)
	// TObjectPtr<APlayerState> RecorderPlayerState;

	// UFUNCTION()
	// void OnRep_RecorderPlayerState();
};
