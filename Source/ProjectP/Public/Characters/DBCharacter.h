// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "PaperZDCharacter.h"
#include "Core/Interfaces/I_Inventory.h"
#include "DBCharacter.generated.h"

class UDBEquipmentComponent;
class UAC_GASHelper;
class UDBInventoryComponent;
class UAC_Inventory;
class UAbilitySystemComponent;
class ADBPlayerState;
class ADBPlayerController;
class UDBPawnExtensionComponent;
class UDBAbilitySystemComponent;

/**
 *	ADBCharacter
 *
 *	The base character pawn class used by this project.
 *	Responsible for sending events to pawn components.
 *	New behavior should be added via pawn components when possible.
 */
UCLASS()
class PROJECTP_API ADBCharacter : public APaperZDCharacter, public IAbilitySystemInterface, public IGameplayCueInterface, public IGameplayTagAssetInterface, public II_Inventory /*, public IDBTeamAgentInterface*/
{
	GENERATED_BODY()

public:
	
	// Sets default values for this character's properties
	ADBCharacter(const FObjectInitializer& ObjectInitializer);


public:

	UFUNCTION(BlueprintCallable, Category = "DB|Character")
	ADBPlayerController* GetDBPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "DB|Character")
	ADBPlayerState* GetDBPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "DB|Character")
	UDBAbilitySystemComponent* GetDBAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

	// TODO: using built-in Crouch?
	// void ToggleCrouch();

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	//~End of AActor interface

	//~APawn interface
	virtual void NotifyControllerChanged() override;
	//~End of APawn interface
	
	// //~IDBTeamAgentInterface interface
	// virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	// virtual FGenericTeamId GetGenericTeamId() const override;
	// virtual FOnDBTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	// //~End of IDBTeamAgentInterface interface
	//
	// /** RPCs that is called on frames when default property replication is skipped. This replicates a single movement update to everyone. */
	// UFUNCTION(NetMulticast, unreliable)
	// void FastSharedReplication(const FSharedRepMovement& SharedRepMovement);
	//
	// // Last FSharedRepMovement we sent, to avoid sending repeatedly.
	// FSharedRepMovement LastSharedReplication;
	//
	// virtual bool UpdateSharedReplication();

	//~II_Inventory interface
	virtual void GetInventoryComponent_Implementation(UAC_Inventory*& Component) override;
	//~End of II_Inventory interface
	
	
protected:

	virtual void OnAbilitySystemInitialized();
	virtual void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void InitializeGameplayTags();

	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	// Begins the death sequence for the character (disables collision, disables movement, etc...)
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	// Ends the death sequence for the character (detaches controller, destroys pawn, etc...)
	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();

	// Called when the death sequence for the character has completed
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual bool CanJumpInternal_Implementation() const;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDBPawnExtensionComponent> PawnExtComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAC_GASHelper> GASHelperComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDBInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDBEquipmentComponent> EquipmentComponent;
	
	
protected:
	
	// // Called to determine what happens to the team ID when possession ends
	// virtual FGenericTeamId DetermineNewTeamAfterPossessionEnds(FGenericTeamId OldTeamID) const
	// {
	// 	// This could be changed to return, e.g., OldTeamID if you want to keep it assigned afterwards, or return an ID for some neutral faction, or etc...
	// 	return FGenericTeamId::NoTeam;
	// }

private:
	
	// UFUNCTION()
	// void OnControllerChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);
	//
	// UFUNCTION()
	// void OnRep_ReplicatedAcceleration();
	//
	// UFUNCTION()
	// void OnRep_MyTeamID(FGenericTeamId OldTeamID);
};
