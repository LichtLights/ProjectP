// Fill out your copyright notice in the Description page of Project Settings.


#include "System/GameModes/DBGameMode.h"

#include "Characters/DBCharacter.h"
#include "Characters/Components/DBPawnExtensionComponent.h"
#include "Development/DBDeveloperSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "LogChannel/DBLogChannels.h"
#include "System/GameModes/DBExperienceDefinition.h"
#include "System/GameModes/DBExperienceManagerComponent.h"
#include "System/GameModes/DBGameState.h"
#include "System/GameModes/DBWorldSettings.h"
#include "System/GameSessions/DBGameSession.h"
#include "System/ManagerClasses/DBAssetManager.h"
#include "System/Player/PlayerState/DBPlayerState.h"
#include "System/PlayerController/DBPlayerController.h"
#include "System/PrimaryData/DBPawnData.h"
#include "UI/DBHUD.h"

ADBGameMode::ADBGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ADBGameState::StaticClass();
	GameSessionClass = ADBGameSession::StaticClass();
	PlayerControllerClass = ADBPlayerController::StaticClass();
	// TODO: Consider implementing ReplaySpectatorPlayerControllerClass.
	// ReplaySpectatorPlayerControllerClass = ADBReplayPlayerController::StaticClass();
	PlayerStateClass = ADBPlayerState::StaticClass();
	DefaultPawnClass = ADBCharacter::StaticClass();
	HUDClass = ADBHUD::StaticClass();
}

const UDBPawnData* ADBGameMode::GetPawnDataForController(const AController* InController) const
{
	// See if pawn data is already set on the player state
	if (InController != nullptr)
	{
		if (const ADBPlayerState* DBPS = InController->GetPlayerState<ADBPlayerState>())
		{
			if (const UDBPawnData* PawnData = DBPS->GetPawnData<UDBPawnData>())
			{
				return PawnData;
			}
		}
	}

	// If not, fall back to the the default for the current experience
	check(GameState);
	UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
	check(ExperienceComponent);

	if (ExperienceComponent->IsExperienceLoaded())
	{
		const UDBExperienceDefinition* Experience = ExperienceComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData != nullptr)
		{
			return Experience->DefaultPawnData;
		}

		// Experience is loaded and there's still no pawn data, fall back to the default for now
		return UDBAssetManager::Get().GetDefaultPawnData();
	}

	// Experience not loaded yet, so there is no pawn data to be had
	return nullptr;
}

void ADBGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// Wait for the next frame to give time to initialize startup settings
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

UClass* ADBGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UDBPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

APawn* ADBGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UDBPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
				else
				{
					UE_LOG(LogDB, Error, TEXT("Game mode was unable to set PawnData on the spawned pawn [%s]."), *GetNameSafe(SpawnedPawn));
				}
			}

			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogDB, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogDB, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

bool ADBGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	// We never want to use the start spot, always use the spawn management component.
	return false;
}

void ADBGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// TODO: Should use this??
	
	// Delay starting new players until the experience has been loaded
	// (players who log in prior to that will be started by OnExperienceLoaded)
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

AActor* ADBGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// TODO: Implement DBPlayerSpawningManagerComponent.
	// if (UDBPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UDBPlayerSpawningManagerComponent>())
	// {
	// 	return PlayerSpawningComponent->ChoosePlayerStart(Player);
	// }
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ADBGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	// TODO: Implement DBPlayerSpawningManagerComponent.
	// if (UDBPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UDBPlayerSpawningManagerComponent>())
	// {
	// 	PlayerSpawningComponent->FinishRestartPlayer(NewPlayer, StartRotation);
	// }

	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool ADBGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

void ADBGameMode::InitGameState()
{
	Super::InitGameState();

	// Listen for the experience load to complete	
	UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnDBExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

bool ADBGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	// Do nothing, we'll wait until PostLogin when we try to spawn the player for real.
	// Doing anything right now is no good, systems like team assignment haven't even occurred yet.
	return true;
}

void ADBGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void ADBGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	// If we tried to spawn a pawn and it failed, lets try again *note* check if there's actually a pawn class
	// before we try this forever.
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			// If it's a player don't loop forever, maybe something changed and they can no longer restart if so stop trying.
			if (PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);			
			}
			else
			{
				UE_LOG(LogDB, Verbose, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(LogDB, Verbose, TEXT("FailedToRestartPlayer(%s) but there's no pawn class so giving up."), *GetPathNameSafe(NewPlayer));
	}
}

void ADBGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	// TODO: change AIController.
	// else if (ADBPlayerBotController* BotController = Cast<ADBPlayerBotController>(Controller))
	// {
	// 	GetWorldTimerManager().SetTimerForNextTick(BotController, &ADBPlayerBotController::ServerRestartController);
	// }
}

bool ADBGameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{	
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	// TODO: Implement DBPlayerSpawningManagerComponent.
	// if (UDBPlayerSpawningManagerComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UDBPlayerSpawningManagerComponent>())
	// {
	// 	return PlayerSpawningComponent->ControllerCanRestart(Controller);
	// }

	return true;
}

void ADBGameMode::OnExperienceLoaded(const UDBExperienceDefinition* CurrentExperience)
{
	// Spawn any players that are already attached
	//@TODO: Here we're handling only *player* controllers, but in GetDefaultPawnClassForController_Implementation we skipped all controllers
	// GetDefaultPawnClassForController_Implementation might only be getting called for players anyways
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);
		if ((PC != nullptr) && (PC->GetPawn() == nullptr))
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

bool ADBGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
	check(ExperienceComponent);

	return ExperienceComponent->IsExperienceLoaded();
}

void ADBGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId, const FString& ExperienceIdSource)
{
	if (ExperienceId.IsValid())
	{
		UE_LOG(LogDBExperience, Log, TEXT("Identified experience %s (Source: %s)"), *ExperienceId.ToString(), *ExperienceIdSource);

		UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogDBExperience, Error, TEXT("Failed to identify experience, loading screen will stay up forever"));
	}
}

void ADBGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	FPrimaryAssetId ExperienceId;
	FString ExperienceIdSource;

	// Precedence order (highest wins)
	//  - Matchmaking assignment (if present)
	//  - URL Options override
	//  - Developer Settings (PIE only)
	//  - Command Line override
	//  - World Settings
	//  - Dedicated server
	//  - Default experience

	UWorld* World = GetWorld();

	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UDBExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
		ExperienceIdSource = TEXT("OptionsString");
	}

	if (!ExperienceId.IsValid() && World->IsPlayInEditor())
	{
		ExperienceId = GetDefault<UDBDeveloperSettings>()->ExperienceOverride;
		ExperienceIdSource = TEXT("DeveloperSettings");
	}

	// see if the command line wants to set the experience
	if (!ExperienceId.IsValid())
	{
		FString ExperienceFromCommandLine;
		if (FParse::Value(FCommandLine::Get(), TEXT("Experience="), ExperienceFromCommandLine))
		{
			ExperienceId = FPrimaryAssetId::ParseTypeAndName(ExperienceFromCommandLine);
			if (!ExperienceId.PrimaryAssetType.IsValid())
			{
				ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UDBExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromCommandLine));
			}
			ExperienceIdSource = TEXT("CommandLine");
		}
	}

	// see if the world settings has a default experience
	if (!ExperienceId.IsValid())
	{
		if (ADBWorldSettings* TypedWorldSettings = Cast<ADBWorldSettings>(GetWorldSettings()))
		{
			ExperienceId = TypedWorldSettings->GetDefaultGameplayExperience();
			ExperienceIdSource = TEXT("WorldSettings");
		}
	}

	UDBAssetManager& AssetManager = UDBAssetManager::Get();
	FAssetData Dummy;
	if (ExperienceId.IsValid() && !AssetManager.GetPrimaryAssetData(ExperienceId, /*out*/ Dummy))
	{
		UE_LOG(LogDBExperience, Error, TEXT("EXPERIENCE: Wanted to use %s but couldn't find it, falling back to the default)"), *ExperienceId.ToString());
		ExperienceId = FPrimaryAssetId();
	}

	// Final fallback to the default experience
	if (!ExperienceId.IsValid())
	{
		// if (TryDedicatedServerLogin())
		
		//@TODO: Pull this from a config setting or something
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("DBExperienceDefinition"), FName("BP_DBDefaultExperience"));
		ExperienceIdSource = TEXT("Default");
	}

	OnMatchAssignmentGiven(ExperienceId, ExperienceIdSource);
}

// TODO: Server play-related functions.
// bool ADBGameMode::TryDedicatedServerLogin()
// {
// 	// Some basic code to register as an active dedicated server, this would be heavily modified by the game
// 	FString DefaultMap = UGameMapsSettings::GetGameDefaultMap();
// 	UWorld* World = GetWorld();
// 	UGameInstance* GameInstance = GetGameInstance();
// 	if (GameInstance && World && World->GetNetMode() == NM_DedicatedServer && World->URL.Map == DefaultMap)
// 	{
// 		// Only register if this is the default map on a dedicated server
// 		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
//
// 		// Dedicated servers may need to do an online login
// 		UserSubsystem->OnUserInitializeComplete.AddDynamic(this, &ADBGameMode::OnUserInitializedForDedicatedServer);
//
// 		// There are no local users on dedicated server, but index 0 means the default platform user which is handled by the online login code
// 		if (!UserSubsystem->TryToLoginForOnlinePlay(0))
// 		{
// 			OnUserInitializedForDedicatedServer(nullptr, false, FText(), ECommonUserPrivilege::CanPlayOnline, ECommonUserOnlineContext::Default);
// 		}
//
// 		return true;
// 	}
//
// 	return false;
// }

// void ADBGameMode::HostDedicatedServerMatch(ECommonSessionOnlineMode OnlineMode)
// {
// 	FPrimaryAssetType UserExperienceType = UDBUserFacingExperienceDefinition::StaticClass()->GetFName();
// 	
// 	// Figure out what UserFacingExperience to load
// 	FPrimaryAssetId UserExperienceId;
// 	FString UserExperienceFromCommandLine;
// 	if (FParse::Value(FCommandLine::Get(), TEXT("UserExperience="), UserExperienceFromCommandLine) ||
// 		FParse::Value(FCommandLine::Get(), TEXT("Playlist="), UserExperienceFromCommandLine))
// 	{
// 		UserExperienceId = FPrimaryAssetId::ParseTypeAndName(UserExperienceFromCommandLine);
// 		if (!UserExperienceId.PrimaryAssetType.IsValid())
// 		{
// 			UserExperienceId = FPrimaryAssetId(FPrimaryAssetType(UserExperienceType), FName(*UserExperienceFromCommandLine));
// 		}
// 	}
//
// 	// Search for the matching experience, it's fine to force load them because we're in dedicated server startup
// 	UDBAssetManager& AssetManager = UDBAssetManager::Get();
// 	TSharedPtr<FStreamableHandle> Handle = AssetManager.LoadPrimaryAssetsWithType(UserExperienceType);
// 	if (ensure(Handle.IsValid()))
// 	{
// 		Handle->WaitUntilComplete();
// 	}
//
// 	TArray<UObject*> UserExperiences;
// 	AssetManager.GetPrimaryAssetObjectList(UserExperienceType, UserExperiences);
// 	UDBUserFacingExperienceDefinition* FoundExperience = nullptr;
// 	UDBUserFacingExperienceDefinition* DefaultExperience = nullptr;
//
// 	for (UObject* Object : UserExperiences)
// 	{
// 		UDBUserFacingExperienceDefinition* UserExperience = Cast<UDBUserFacingExperienceDefinition>(Object);
// 		if (ensure(UserExperience))
// 		{
// 			if (UserExperience->GetPrimaryAssetId() == UserExperienceId)
// 			{
// 				FoundExperience = UserExperience;
// 				break;
// 			}
// 			
// 			if (UserExperience->bIsDefaultExperience && DefaultExperience == nullptr)
// 			{
// 				DefaultExperience = UserExperience;
// 			}
// 		}
// 	}
//
// 	if (FoundExperience == nullptr)
// 	{
// 		FoundExperience = DefaultExperience;
// 	}
// 	
// 	UGameInstance* GameInstance = GetGameInstance();
// 	if (ensure(FoundExperience && GameInstance))
// 	{
// 		// Actually host the game
// 		UCommonSession_HostSessionRequest* HostRequest = FoundExperience->CreateHostingRequest(this);
// 		if (ensure(HostRequest))
// 		{
// 			HostRequest->OnlineMode = OnlineMode;
//
// 			// TODO override other parameters?
//
// 			UCommonSessionSubsystem* SessionSubsystem = GameInstance->GetSubsystem<UCommonSessionSubsystem>();
// 			SessionSubsystem->HostSession(nullptr, HostRequest);
// 			
// 			// This will handle the map travel
// 		}
// 	}
//
// }

// void ADBGameMode::OnUserInitializedForDedicatedServer(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error,
// 	ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
// {
// 	UGameInstance* GameInstance = GetGameInstance();
// 	if (GameInstance)
// 	{
// 		// Unbind
// 		UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
// 		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &ADBGameMode::OnUserInitializedForDedicatedServer);
//
// 		// Dedicated servers do not require user login, but some online subsystems may expect it
// 		if (bSuccess && ensure(UserInfo))
// 		{
// 			UE_LOG(LogDBExperience, Log, TEXT("Dedicated server user login succeeded for id %s, starting online server"), *UserInfo->GetNetId().ToString());
// 		}
// 		else
// 		{
// 			UE_LOG(LogDBExperience, Log, TEXT("Dedicated server user login unsuccessful, starting online server as login is not required"));
// 		}
// 		
// 		HostDedicatedServerMatch(ECommonSessionOnlineMode::Online);
// 	}
// }
