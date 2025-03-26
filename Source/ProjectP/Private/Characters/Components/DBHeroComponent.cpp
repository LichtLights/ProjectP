// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/DBHeroComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "Characters/DBCharacter.h"
#include "Characters/Components/DBPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DBGameplayTags.h"
#include "Input/DBInputComponent.h"
#include "LogChannel/DBLogChannels.h"
#include "System/Camera/DBCameraComponent.h"
#include "System/Camera/DBCameraMode.h"
#include "System/Player/DBLocalPlayer.h"
#include "System/Player/PlayerState/DBPlayerState.h"
#include "System/PlayerController/DBPlayerController.h"
#include "System/PrimaryData/DBPawnData.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#if WITH_EDITOR
#include "Misc/UObjectToken.h"
#endif	// WITH_EDITOR

namespace DBHero
{
	// TODO: Move GlobalConstants here?...
	// static const float LookYawRate = 300.0f;
	// static const float LookPitchRate = 165.0f;
};

const FName UDBHeroComponent::NAME_BindInputsNow("BindInputsNow");
const FName UDBHeroComponent::NAME_ActorFeatureName("Hero");

UDBHeroComponent::UDBHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UDBHeroComponent::SetAbilityCameraMode(TSubclassOf<UDBCameraMode> CameraMode,
	const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UDBHeroComponent::ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (AbilityCameraModeOwningSpecHandle == OwningSpecHandle)
	{
		AbilityCameraMode = nullptr;
		AbilityCameraModeOwningSpecHandle = FGameplayAbilitySpecHandle();
	}
}

void UDBHeroComponent::AddAdditionalInputConfig(const UDBInputConfig* InputConfig)
{
	TArray<uint32> BindHandles;

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (const UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		UDBInputComponent* DBIC = Pawn->FindComponentByClass<UDBInputComponent>();
		if (ensureMsgf(DBIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UDBInputComponent or a subclass of it.")))
		{
			DBIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
		}
	}
}

void UDBHeroComponent::RemoveAdditionalInputConfig(const UDBInputConfig* InputConfig)
{
	//@TODO: Implement me!
}

bool UDBHeroComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

bool UDBHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if (!CurrentState.IsValid() && DesiredState == DBGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if (Pawn)
		{
			return true;
		}
	}
	else if (CurrentState == DBGameplayTags::InitState_Spawned && DesiredState == DBGameplayTags::InitState_DataAvailable)
	{
		// The player state is required.
		if (!GetPlayerState<ADBPlayerState>())
		{
			return false;
		}

		// If we're authority or autonomous, we need to wait for a controller with registered ownership of the player state.
		if (Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if (!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if (bIsLocallyControlled && !bIsBot)
		{
			ADBPlayerController* DBPC = GetController<ADBPlayerController>();

			// The input component and local player is required when locally controlled.
			if (!Pawn->InputComponent || !DBPC || !DBPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if (CurrentState == DBGameplayTags::InitState_DataAvailable && DesiredState == DBGameplayTags::InitState_DataInitialized)
	{
		// Wait for player state and extension component
		ADBPlayerState* DBPS = GetPlayerState<ADBPlayerState>();
		
		return DBPS && Manager->HasFeatureReachedInitState(Pawn, UDBPawnExtensionComponent::NAME_ActorFeatureName, DBGameplayTags::InitState_DataInitialized);
	}
	else if (CurrentState == DBGameplayTags::InitState_DataInitialized && DesiredState == DBGameplayTags::InitState_GameplayReady)
	{
		// TODO: add ability initialization checks?
		
		return true;
	}

	return false;
}

void UDBHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	if (CurrentState == DBGameplayTags::InitState_DataAvailable && DesiredState == DBGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ADBPlayerState* DBPS = GetPlayerState<ADBPlayerState>();
		if (!ensure(Pawn && DBPS))
		{
			return;
		}
		
		const UDBPawnData* PawnData = nullptr;
		
		if (UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UDBPawnData>();
		
			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			PawnExtComp->InitializeAbilitySystem(DBPS->GetDBAbilitySystemComponent(), DBPS);
		}
		
		if (ADBPlayerController* DBPC = GetController<ADBPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
		
		// Hook up the delegate for all pawns, in case we spectate later
		if (PawnData)
		{
			if (UDBCameraComponent* CameraComponent = UDBCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
	}
}

void UDBHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UDBPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == DBGameplayTags::InitState_DataInitialized)
		{
			// If the extension component says all all other components are initialized, try to progress to next state
			CheckDefaultInitialization();
		}
	}
}

void UDBHeroComponent::CheckDefaultInitialization()
{
	static const TArray<FGameplayTag> StateChain = {
		DBGameplayTags::InitState_Spawned,
		DBGameplayTags::InitState_DataAvailable,
		DBGameplayTags::InitState_DataInitialized,
		DBGameplayTags::InitState_GameplayReady
	};

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UDBHeroComponent::OnRegister()
{
	Super::OnRegister();

	if (!GetPawn<APawn>())
	{
		UE_LOG(LogDB, Error, TEXT("[UDBHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));

#if WITH_EDITOR
		if (GIsEditor)
		{
			static const FText Message = NSLOCTEXT("DBHeroComponent", "NotOnPawnError", "has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint. This will cause a crash if you PIE!");
			static const FName HeroMessageLogName = TEXT("DBHeroComponent");
			
			FMessageLog(HeroMessageLogName).Error()
				->AddToken(FUObjectToken::Create(this, FText::FromString(GetNameSafe(this))))
				->AddToken(FTextToken::Create(Message));
				
			FMessageLog(HeroMessageLogName).Open();
		}
#endif
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game world
		RegisterInitStateFeature();
	}
}

void UDBHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension component changes init state
	BindOnActorInitStateChanged(UDBPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning, then try the rest of initialization
	ensure(TryToChangeInitState(DBGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UDBHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UDBHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const UDBLocalPlayer* LP = Cast<UDBLocalPlayer>(PC->GetLocalPlayer());
	check(LP);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);
	
	Subsystem->ClearAllMappings();
	
	if (const UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UDBPawnData* PawnData = PawnExtComp->GetPawnData<UDBPawnData>())
		{
			if (const UDBInputConfig* InputConfig = PawnData->InputConfig)
			{
				for (const FInputMappingContextAndPriority& Mapping : DefaultInputMappings)
				{
					if (UInputMappingContext* IMC = Mapping.InputMapping.Get())
					{
						if (Mapping.bRegisterWithSettings)
						{
							if (UEnhancedInputUserSettings* Settings = Subsystem->GetUserSettings())
							{
								Settings->RegisterInputMappingContext(IMC);
							}
							
							FModifyContextOptions Options = {};
							Options.bIgnoreAllPressedKeysUntilRelease = false;
							// Actually add the config to the local player							
							Subsystem->AddMappingContext(IMC, Mapping.Priority, Options);
						}
					}
				}
	
				// The DB Input Component has some additional functions to map Gameplay Tags to an Input Action.
				// If you want this functionality but still want to change your input component class, make it a subclass
				// of the UDBInputComponent or modify this component accordingly.
				UDBInputComponent* DBIC = Cast<UDBInputComponent>(PlayerInputComponent);
				if (ensureMsgf(DBIC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UDBInputComponent or a subclass of it.")))
				{
					// Add the key mappings that may have been set by the player
					DBIC->AddInputMappings(InputConfig, Subsystem);
	
					// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
					// be triggered directly by these input actions Triggered events. 
					TArray<uint32> BindHandles;
					DBIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);
	
					DBIC->BindNativeAction(InputConfig, DBGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
					DBIC->BindNativeAction(InputConfig, DBGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
					DBIC->BindNativeAction(InputConfig, DBGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
					DBIC->BindNativeAction(InputConfig, DBGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
					// DBIC->BindNativeAction(InputConfig, DBGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
				}
			}
		}
	}
	
	if (ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}
 
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UDBHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UDBAbilitySystemComponent* DBASC = PawnExtComp->GetDBAbilitySystemComponent())
			{
				DBASC->AbilityInputTagPressed(InputTag);
			}
		}	
	}
}

void UDBHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	if (const UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (UDBAbilitySystemComponent* DBASC = PawnExtComp->GetDBAbilitySystemComponent())
		{
			DBASC->AbilityInputTagReleased(InputTag);
		}
	}
}

void UDBHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		// Ensure the Value is a 2D vector
		FVector2D MoveVector = InputActionValue.Get<FVector2D>();

		// Apply movement based on the MoveVector
		if (!MoveVector.IsZero())
		{
			FVector Direction = FVector(MoveVector, 0.0f);
			Direction.Normalize();
			// TODO: get MovementSpeed modifier from ASC AttributeSet.
			// AddMovementInput(Direction, StatusComp->CurrentMovementSpeed);
			Pawn->AddMovementInput(Direction, 300.f);
		}
	}
	
	// // If the player has attempted to move again then cancel auto running
	// if (ADBPlayerController* DBController = Cast<ADBPlayerController>(Controller))
	// {
	// 	DBController->SetIsAutoRunning(false);
	// }
	//
	// if (Controller)
	// {
	// 	const FVector2D Value = InputActionValue.Get<FVector2D>();
	// 	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	//
	// 	if (Value.X != 0.0f)
	// 	{
	// 		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
	// 		Pawn->AddMovementInput(MovementDirection, Value.X);
	// 	}
	//
	// 	if (Value.Y != 0.0f)
	// 	{
	// 		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
	// 		Pawn->AddMovementInput(MovementDirection, Value.Y);
	// 	}
	// }
}

void UDBHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();

	if (!Pawn)
	{
		return;
	}
	
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UDBHeroComponent::Input_LookStick(const FInputActionValue& InputActionValue)
{
	// APawn* Pawn = GetPawn<APawn>();
	//
	// if (!Pawn)
	// {
	// 	return;
	// }
	//
	// const FVector2D Value = InputActionValue.Get<FVector2D>();
	//
	// const UWorld* World = GetWorld();
	// check(World);
	//
	// if (Value.X != 0.0f)
	// {
	// 	Pawn->AddControllerYawInput(Value.X * DBHero::LookYawRate * World->GetDeltaSeconds());
	// }
	//
	// if (Value.Y != 0.0f)
	// {
	// 	Pawn->AddControllerPitchInput(Value.Y * DBHero::LookPitchRate * World->GetDeltaSeconds());
	// }
}

void UDBHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ADBCharacter* Character = GetPawn<ADBCharacter>())
	{
		// TODO: Implement Crouch specific to paperZD Character.
		// Character->ToggleCrouch();
	}
}

TSubclassOf<UDBCameraMode> UDBHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UDBPawnExtensionComponent* PawnExtComp = UDBPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UDBPawnData* PawnData = PawnExtComp->GetPawnData<UDBPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

// void UDBHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
// {
// }
