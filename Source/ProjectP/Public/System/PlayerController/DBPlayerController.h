// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "DBPlayerController.generated.h"

class UDBIndicatorManagerComponent;
class UDBSettingsShared;
class ADBHUD;
class UDBAbilitySystemComponent;
class ADBPlayerState;
enum class EPopUpWindowRelation : uint8;
class UInteractionWidget;
class UItemContextMenu;
class UInventoryDragDropOp;
class UGridItem;
struct FInputActionValue;

/**
 *	ADBPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class PROJECTP_API ADBPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DB|PlayerController", Meta = (AllowPrivateAccess = "true"))
	UDBIndicatorManagerComponent* IndicatorManagerComp;
	
	
public:

	ADBPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "DB|PlayerController")
	ADBPlayerState* GetDBPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "DB|PlayerController")
	UDBAbilitySystemComponent* GetDBAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "DB|PlayerController")
	ADBHUD* GetDBHUD() const;

	// TODO: ReplayController related.
	// // Call from game state logic to start recording an automatic client replay if ShouldRecordClientReplay returns true
	// UFUNCTION(BlueprintCallable, Category = "DB|PlayerController")
	// bool TryToRecordClientReplay();
	//
	// // Call to see if we should record a replay, subclasses could change this
	// virtual bool ShouldRecordClientReplay();
	
	// TODO: Cheat, Server related.
	// // Run a cheat command on the server.
	// UFUNCTION(Reliable, Server, WithValidation)
	// void ServerCheat(const FString& Msg);
	//
	// // Run a cheat command on the server for all players.
	// UFUNCTION(Reliable, Server, WithValidation)
	// void ServerCheatAll(const FString& Msg)

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~End of AActor interface

	//~AController interface
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~APlayerController interface
	virtual void ReceivedPlayer() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetPlayer(UPlayer* InPlayer) override;
	// virtual void AddCheats(bool bForce) override;
	virtual void UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	// TODO: Camera related.
	// //~IDBCameraAssistInterface interface
	// virtual void OnCameraPenetratingTarget() override;
	// //~End of IDBCameraAssistInterface interface

	// TODO: TeamInterface related.
	// //~IDBTeamAgentInterface interface
	// virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	// virtual FGenericTeamId GetGenericTeamId() const override;
	// virtual FOnDBTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	// //~End of IDBTeamAgentInterface interface

	// TODO: AutoRunning related.
	// UFUNCTION(BlueprintCallable, Category = "DB|Character")
	// void SetIsAutoRunning(const bool bEnabled);
	//
	// UFUNCTION(BlueprintCallable, Category = "DB|Character")
	// bool GetIsAutoRunning() const;
	
private:
	
	// UPROPERTY()
	// FOnDBTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;

private:
	
	// UFUNCTION()
	// void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

protected:
	
	// Called when the player state is set or cleared
	virtual void OnPlayerStateChanged();

private:

	// TODO: TeamInterface related.
	// void BroadcastOnPlayerStateChanged();

protected:

	//~APlayerController interface

	//~End of APlayerController interface

	void OnSettingsChanged(UDBSettingsShared* InSettings);

	// TODO: AutoRunning related.
	// void OnStartAutoRun();
	// void OnEndAutoRun();
	//
	// UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStartAutoRun"))
	// void K2_OnStartAutoRun();
	//
	// UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnEndAutoRun"))
	// void K2_OnEndAutoRun();

	bool bHideViewTargetPawnNextFrame = false;



	
	

//////////////////////////////////////////////////
/// DEPRECATED code below
	
private:

	// UI handle functions
	void ToggleCharacterMenu();

	void OpenInventory();
	void OpenWeaponMastery();

	void OnTabPressed();

	void OnInteract();
	
	void OnRotateItem();

	
	// TODO: DEBUG only.	
	void ToggleDebug();
	bool bIsDebugModeOn = true;
};

// A player controller used for replay capture and playback
// UCLASS()
// class ADBReplayPlayerController : public ADBPlayerController
// {
// 	GENERATED_BODY()
//
// 	virtual void Tick(float DeltaSeconds) override;
// 	virtual void SmoothTargetViewRotation(APawn* TargetPawn, float DeltaSeconds) override;
// 	virtual bool ShouldRecordClientReplay() override;
//
// 	// Callback for when the game state's RecorderPlayerState gets replicated during replay playback
// 	void RecorderPlayerStateUpdated(APlayerState* NewRecorderPlayerState);
//
// 	// Callback for when the followed player state changes pawn
// 	UFUNCTION()
// 	void OnPlayerStatePawnSet(APlayerState* ChangedPlayerState, APawn* NewPlayerPawn, APawn* OldPlayerPawn);
//
// 	// The player state we are currently following */
// 	UPROPERTY(Transient)
// 	TObjectPtr<APlayerState> FollowedPlayerState;
// };
