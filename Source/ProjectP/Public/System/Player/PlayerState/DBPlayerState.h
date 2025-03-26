// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/GameplayTagStack.h"
#include "DBPlayerState.generated.h"

struct FDBVerbMessage;
class UDBExperienceDefinition;
struct FGameplayTag;
class UDBPawnData;
class UDBAbilitySystemComponent;
class ADBPlayerController;

/** Defines the types of client connected */
UENUM()
enum class EDBPlayerConnectionType : uint8
{
	// An active player
	Player = 0,

	// Spectator connected to a running game
	LiveSpectator,

	// Spectating a demo recording offline
	ReplaySpectator,

	// A deactivated player (disconnected)
	InactivePlayer
};

/**
 *	ADBPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class PROJECTP_API ADBPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ADBPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DB|PlayerState")
	ADBPlayerController* GetDBPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "DB|PlayerState")
	UDBAbilitySystemComponent* GetDBAbilitySystemComponent() const; // Get DBAbilitySystemComponent from Character directly
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UDBPawnData* InPawnData);

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void Reset() override;
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface

	// //~IDBTeamAgentInterface interface
	// virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	// virtual FGenericTeamId GetGenericTeamId() const override;
	// virtual FOnDBTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	// //~End of IDBTeamAgentInterface interface

	static const FName NAME_DBAbilityReady;

	void SetPlayerConnectionType(EDBPlayerConnectionType NewType);
	EDBPlayerConnectionType GetPlayerConnectionType() const { return MyPlayerConnectionType; }

	// /** Returns the Squad ID of the squad the player belongs to. */
	// UFUNCTION(BlueprintCallable)
	// int32 GetSquadId() const
	// {
	// 	return MySquadID;
	// }
	//
	// /** Returns the Team ID of the team the player belongs to. */
	// UFUNCTION(BlueprintCallable)
	// int32 GetTeamId() const
	// {
	// 	return GenericTeamIdToInteger(MyTeamID);
	// }
	//
	// void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category=Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category=Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "DB|PlayerState")
	void ClientBroadcastMessage(const FDBVerbMessage Message);

	// // Gets the replicated view rotation of this player, used for spectating
	// FRotator GetReplicatedViewRotation() const;
	//
	// // Sets the replicated view rotation, only valid on the server
	// void SetReplicatedViewRotation(const FRotator& NewRotation);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	
	void OnExperienceLoaded(const UDBExperienceDefinition* CurrentExperience);

	
protected:
	
	UFUNCTION()
	void OnRep_PawnData();
	

protected:

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UDBPawnData> PawnData;


private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "DB|PlayerState")
	TObjectPtr<UDBAbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UDBStatusAttributeSet> StatusSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UDBCombatSet> CombatSet;

	UPROPERTY(Replicated)
	EDBPlayerConnectionType MyPlayerConnectionType;

	// UPROPERTY()
	// FOnDBTeamIndexChangedDelegate OnTeamChangedDelegate;

	// UPROPERTY(ReplicatedUsing=OnRep_MyTeamID)
	// FGenericTeamId MyTeamID;

	// UPROPERTY(ReplicatedUsing=OnRep_MySquadID)
	// int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	// UPROPERTY(Replicated)
	// FRotator ReplicatedViewRotation;

	
private:
	
	// UFUNCTION()
	// void OnRep_MyTeamID(FGenericTeamId OldTeamID);
	//
	// UFUNCTION()
	// void OnRep_MySquadID();
};
