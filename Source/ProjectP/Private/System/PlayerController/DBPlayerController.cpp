// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PlayerController/DBPlayerController.h"

#include "AbilitySystemGlobals.h"
#include "CommonInputSubsystem.h"
#include "GlobalConstants.h"
#include "WidgetUtilityFunctions.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "GameplayTags/DBGameplayTags.h"
#include "LogChannel/DBLogChannels.h"
#include "System/Player/DBLocalPlayer.h"
#include "System/Player/PlayerState/DBPlayerState.h"
#include "System/Settings/DBSettingsShared.h"
#include "UI/DBHUD.h"
#include "UI/IndicatorSystem/DBIndicatorManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBPlayerController)

namespace DB
{
	namespace Input
	{
		static int32 ShouldAlwaysPlayForceFeedback = 0;
		static FAutoConsoleVariableRef CVarShouldAlwaysPlayForceFeedback(TEXT("DBPC.ShouldAlwaysPlayForceFeedback"),
			ShouldAlwaysPlayForceFeedback,
			TEXT("Should force feedback effects be played, even if the last input device was not a gamepad?"));
	}
}

ADBPlayerController::ADBPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IndicatorManagerComp = CreateDefaultSubobject<UDBIndicatorManagerComponent>(TEXT("Indicator Manager"));
}

ADBPlayerState* ADBPlayerController::GetDBPlayerState() const
{
	return CastChecked<ADBPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UDBAbilitySystemComponent* ADBPlayerController::GetDBAbilitySystemComponent() const
{
	ADBPlayerCharacter* const DBPlayerCharacter = Cast<ADBPlayerCharacter>(GetCharacter());

	return (DBPlayerCharacter ? DBPlayerCharacter->GetDBAbilitySystemComponent() : nullptr);
	
	// const ADBPlayerState* DBPS = GetDBPlayerState();
	// return (DBPS ? DBPS->GetDBAbilitySystemComponent() : nullptr);
}

ADBHUD* ADBPlayerController::GetDBHUD() const
{
	return CastChecked<ADBHUD>(GetHUD(), ECastCheckedType::NullAllowed);
}

void ADBPlayerController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	SetShowMouseCursor(true);
}

void ADBPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ADBPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADBPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// TODO: Server related.
// #if WITH_SERVER_CODE && WITH_EDITOR
// 	if (GIsEditor && (InPawn != nullptr) && (GetPawn() == InPawn))
// 	{
// 		for (const FDBCheatToRun& CheatRow : GetDefault<UDBDeveloperSettings>()->CheatsToRun)
// 		{
// 			if (CheatRow.Phase == ECheatExecutionTime::OnPlayerPawnPossession)
// 			{
// 				ConsoleCommand(CheatRow.Cheat, /*bWriteToLog=*/ true);
// 			}
// 		}
// 	}
// #endif
//
// 	SetIsAutoRunning(false);
}

void ADBPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}
	
	Super::OnUnPossess();
}

void ADBPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	// TODO: TeamInterface related.
	// BroadcastOnPlayerStateChanged();
}

void ADBPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	// TODO: TeamInterface related.
	// BroadcastOnPlayerStateChanged();
}

void ADBPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// TODO: TeamInterface related.
	// BroadcastOnPlayerStateChanged();
}

void ADBPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
}

void ADBPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// TODO: Auto-running related.
	// If we are auto running then add some player input
	// if (GetIsAutoRunning())
	// {
	// 	if (APawn* CurrentPawn = GetPawn())
	// 	{
	// 		const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	// 		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	// 		CurrentPawn->AddMovementInput(MovementDirection, 1.0f);	
	// 	}
	// }

	// TODO: Server-replication related.
	// ADBPlayerState* DBPlayerState = GetDBPlayerState();
	//
	// if (PlayerCameraManager && DBPlayerState)
	// {
	// 	APawn* TargetPawn = PlayerCameraManager->GetViewTargetPawn();
	//
	// 	if (TargetPawn)
	// 	{
	// 		// Update view rotation on the server so it replicates
	// 		if (HasAuthority() || TargetPawn->IsLocallyControlled())
	// 		{
	// 			DBPlayerState->SetReplicatedViewRotation(TargetPawn->GetViewRotation());
	// 		}
	//
	// 		// Update the target view rotation if the pawn isn't locally controlled
	// 		if (!TargetPawn->IsLocallyControlled())
	// 		{
	// 			DBPlayerState = TargetPawn->GetPlayerState<ADBPlayerState>();
	// 			if (DBPlayerState)
	// 			{
	// 				// Get it from the spectated pawn's player state, which may not be the same as the PC's playerstate
	// 				TargetViewRotation = DBPlayerState->GetReplicatedViewRotation();
	// 			}
	// 		}
	// 	}
	// }
}

void ADBPlayerController::SetPlayer(UPlayer* InPlayer)
{
	Super::SetPlayer(InPlayer);

	if (const UDBLocalPlayer* DBLocalPlayer = Cast<UDBLocalPlayer>(InPlayer))
	{
		UDBSettingsShared* UserSettings = DBLocalPlayer->GetSharedSettings();
		UserSettings->OnSettingChanged.AddUObject(this, &ThisClass::OnSettingsChanged);

		OnSettingsChanged(UserSettings);
	}
}

// TODO: Consider using CheatManager.
// void ADBPlayerController::AddCheats(bool bForce)
// {
// #if USING_CHEAT_MANAGER
// 	Super::AddCheats(true);
// #else //#if USING_CHEAT_MANAGER
// 	Super::AddCheats(bForce);
// #endif // #else //#if USING_CHEAT_MANAGER
// }

void ADBPlayerController::UpdateForceFeedback(IInputInterface* InputInterface, const int32 ControllerId)
{
	if (bForceFeedbackEnabled)
	{
		if (const UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(GetLocalPlayer()))
		{
			const ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
			if (DB::Input::ShouldAlwaysPlayForceFeedback || CurrentInputType == ECommonInputType::Gamepad || CurrentInputType == ECommonInputType::Touch)
			{
				InputInterface->SetForceFeedbackChannelValues(ControllerId, ForceFeedbackValues);
				return;
			}
		}
	}
	
	InputInterface->SetForceFeedbackChannelValues(ControllerId, FForceFeedbackValues());
}

void ADBPlayerController::UpdateHiddenComponents(const FVector& ViewLocation,
	TSet<FPrimitiveComponentId>& OutHiddenComponents)
{
	Super::UpdateHiddenComponents(ViewLocation, OutHiddenComponents);

	if (bHideViewTargetPawnNextFrame)
	{
		AActor* const ViewTargetPawn = PlayerCameraManager ? Cast<AActor>(PlayerCameraManager->GetViewTarget()) : nullptr;
		if (ViewTargetPawn)
		{
			// internal helper func to hide all the components
			auto AddToHiddenComponents = [&OutHiddenComponents](const TInlineComponentArray<UPrimitiveComponent*>& InComponents)
			{
				// add every component and all attached children
				for (UPrimitiveComponent* Comp : InComponents)
				{
					if (Comp->IsRegistered())
					{
						OutHiddenComponents.Add(Comp->GetPrimitiveSceneId());

						for (USceneComponent* AttachedChild : Comp->GetAttachChildren())
						{
							static FName NAME_NoParentAutoHide(TEXT("NoParentAutoHide"));
							UPrimitiveComponent* AttachChildPC = Cast<UPrimitiveComponent>(AttachedChild);
							if (AttachChildPC && AttachChildPC->IsRegistered() && !AttachChildPC->ComponentTags.Contains(NAME_NoParentAutoHide))
							{
								OutHiddenComponents.Add(AttachChildPC->GetPrimitiveSceneId());
							}
						}
					}
				}
			};

			//TODO Solve with an interface.  Gather hidden components or something.
			//TODO Hiding isn't awesome, sometimes you want the effect of a fade out over a proximity, needs to bubble up to designers.

			// hide pawn's components
			TInlineComponentArray<UPrimitiveComponent*> PawnComponents;
			ViewTargetPawn->GetComponents(PawnComponents);
			AddToHiddenComponents(PawnComponents);

			//// hide weapon too
			//if (ViewTargetPawn->CurrentWeapon)
			//{
			//	TInlineComponentArray<UPrimitiveComponent*> WeaponComponents;
			//	ViewTargetPawn->CurrentWeapon->GetComponents(WeaponComponents);
			//	AddToHiddenComponents(WeaponComponents);
			//}
		}

		// we consumed it, reset for next frame
		bHideViewTargetPawnNextFrame = false;
	}
}

void ADBPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void ADBPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		DBASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

// void ADBPlayerController::OnCameraPenetratingTarget()
// {
// 	bHideViewTargetPawnNextFrame = true;
// }

// void ADBPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
// {
// 	UE_LOG(LogDBTeams, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
// }
//
// FGenericTeamId ADBPlayerController::GetGenericTeamId() const
// {
// 	if (const IDBTeamAgentInterface* PSWithTeamInterface = Cast<IDBTeamAgentInterface>(PlayerState))
// 	{
// 		return PSWithTeamInterface->GetGenericTeamId();
// 	}
// 	return FGenericTeamId::NoTeam;
// }
//
// FOnDBTeamIndexChangedDelegate* ADBPlayerController::GetOnTeamIndexChangedDelegate()
// {
// 	return &OnTeamChangedDelegate;
// }

// TODO: Consider implementing AutoRunning.
// void ADBPlayerController::SetIsAutoRunning(const bool bEnabled)
// {
// 	const bool bIsAutoRunning = GetIsAutoRunning();
// 	if (bEnabled != bIsAutoRunning)
// 	{
// 		if (!bEnabled)
// 		{
// 			OnEndAutoRun();
// 		}
// 		else
// 		{
// 			OnStartAutoRun();
// 		}
// 	}
// }
//
// bool ADBPlayerController::GetIsAutoRunning() const
// {
// 	bool bIsAutoRunning = false;
// 	if (const UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
// 	{
// 		bIsAutoRunning = DBASC->GetTagCount(DBGameplayTags::Status_AutoRunning) > 0;
// 	}
// 	return bIsAutoRunning;
// }

// void ADBPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
// {
// 	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
// }

void ADBPlayerController::OnPlayerStateChanged()
{
	// Empty, place for derived classes to implement without having to hook all the other events
}

// void ADBPlayerController::BroadcastOnPlayerStateChanged()
// {
// 	OnPlayerStateChanged();
//
// 	// Unbind from the old player state, if any
// 	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
// 	if (LastSeenPlayerState != nullptr)
// 	{
// 		if (IDBTeamAgentInterface* PlayerStateTeamInterface = Cast<IDBTeamAgentInterface>(LastSeenPlayerState))
// 		{
// 			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
// 			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
// 		}
// 	}
//
// 	// Bind to the new player state, if any
// 	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
// 	if (PlayerState != nullptr)
// 	{
// 		if (IDBTeamAgentInterface* PlayerStateTeamInterface = Cast<IDBTeamAgentInterface>(PlayerState))
// 		{
// 			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
// 			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
// 		}
// 	}
//
// 	// Broadcast the team change (if it really has)
// 	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
//
// 	LastSeenPlayerState = PlayerState;
// }

void ADBPlayerController::OnSettingsChanged(UDBSettingsShared* InSettings)
{
	bForceFeedbackEnabled = InSettings->GetForceFeedbackEnabled();
}

// void ADBPlayerController::OnStartAutoRun()
// {
// 	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
// 	{
// 		DBASC->SetLooseGameplayTagCount(DBGameplayTags::Status_AutoRunning, 1);
// 		K2_OnStartAutoRun();
// 	}	
// }
//
// void ADBPlayerController::OnEndAutoRun()
// {
// 	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
// 	{
// 		DBASC->SetLooseGameplayTagCount(DBGameplayTags::Status_AutoRunning, 0);
// 		K2_OnEndAutoRun();
// 	}
// }
