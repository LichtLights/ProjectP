// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DBCharacter.h"

#include "GlobalConstants.h"
#include "PaperFlipbookComponent.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "Characters/Components/DBPawnExtensionComponent.h"
#include "Characters/Components/PaperCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/Components/AC_Inventory.h"
#include "DB_IFPSystem/Inventory/Components/DBEquipmentComponent.h"
#include "DB_IFPSystem/Inventory/Components/DBInventoryComponent.h"
#include "GameplayTags/DBGameplayTags.h"
#include "IFP_GAS/Public/AC_GASHelper.h"
#include "System/Player/PlayerState/DBPlayerState.h"
#include "System/PlayerController/DBPlayerController.h"


ADBCharacter::ADBCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UPaperCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
	)
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetSprite()->CastShadow = true;
	GetSprite()->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantRight);

	// Set the MaskedLit material for the Sprite
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(TEXT("/Game/ProjectP/Materials/SpriteMaterials/MI_MaskedLitSpriteMaterial.MI_MaskedLitSpriteMaterial"));
	if (MaterialAsset.Succeeded())
	{
		GetSprite()->SetMaterial(0, MaterialAsset.Object);
	}

	// Initialize Components
	PawnExtComponent = CreateDefaultSubobject<UDBPawnExtensionComponent>(TEXT("Pawn Extension Component"));
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));

	GASHelperComponent = CreateDefaultSubobject<UAC_GASHelper>(TEXT("GAS Helper Component"));
	
	InventoryComponent = CreateDefaultSubobject<UDBInventoryComponent>(TEXT("Inventory Component"));

	EquipmentComponent = CreateDefaultSubobject<UDBEquipmentComponent>(TEXT("Equipment Component"));
}

ADBPlayerController* ADBCharacter::GetDBPlayerController() const
{
	return CastChecked<ADBPlayerController>(Controller, ECastCheckedType::NullAllowed);
}

ADBPlayerState* ADBCharacter::GetDBPlayerState() const
{
	return CastChecked<ADBPlayerState>(GetPlayerState(), ECastCheckedType::NullAllowed);
}

UDBAbilitySystemComponent* ADBCharacter::GetDBAbilitySystemComponent() const
{
	return Cast<UDBAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ADBCharacter::GetAbilitySystemComponent() const
{
	// Get ASC from PawnExtComponent in default (former Lyra implementation)
	// If for Child character class, overriden with returning ASC(Player, Enemy) directly.
	if (PawnExtComponent == nullptr)
	{
		return nullptr;
	}

	return PawnExtComponent->GetDBAbilitySystemComponent();
}

void ADBCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		DBASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ADBCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		return DBASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ADBCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		return DBASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ADBCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		return DBASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void ADBCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ADBCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// UWorld* World = GetWorld();
	//
	// const bool bRegisterWithSignificanceManager = !IsNetMode(NM_DedicatedServer);
	// if (bRegisterWithSignificanceManager)
	// {
	// 	if (UDBSignificanceManager* SignificanceManager = USignificanceManager::Get<UDBSignificanceManager>(World))
	// 	{
	// 		SignificanceManager->UnregisterObject(this);
	// 	}
	// }
}

void ADBCharacter::Reset()
{
	// Super::Reset();

	// DisableMovementAndCollision();

	K2_OnReset();

	// UninitAndDestroy();
}

void ADBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
	// DOREPLIFETIME(ThisClass, MyTeamID)
}

void ADBCharacter::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	// if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	// {
	// 	// Compress Acceleration: XY components as direction + magnitude, Z component as direct value
	// 	const double MaxAccel = MovementComponent->MaxAcceleration;
	// 	const FVector CurrentAccel = MovementComponent->GetCurrentAcceleration();
	// 	double AccelXYRadians, AccelXYMagnitude;
	// 	FMath::CartesianToPolar(CurrentAccel.X, CurrentAccel.Y, AccelXYMagnitude, AccelXYRadians);
	//
	// 	ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);     // [0, 2PI] -> [0, 255]
	// 	ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);	// [0, MaxAccel] -> [0, 255]
	// 	ReplicatedAcceleration.AccelZ           = FMath::FloorToInt((CurrentAccel.Z / MaxAccel) * 127.0);   // [-MaxAccel, MaxAccel] -> [-127, 127]
	// }
}

void ADBCharacter::NotifyControllerChanged()
{
	// const FGenericTeamId OldTeamId = GetGenericTeamId();

	Super::NotifyControllerChanged();

	// // Update our team ID based on the controller
	// if (HasAuthority() && (Controller != nullptr))
	// {
	// 	if (IDBTeamAgentInterface* ControllerWithTeam = Cast<IDBTeamAgentInterface>(Controller))
	// 	{
	// 		MyTeamID = ControllerWithTeam->GetGenericTeamId();
	// 		ConditionalBroadcastTeamChanged(this, OldTeamId, MyTeamID);
	// 	}
	// }
}

void ADBCharacter::GetInventoryComponent_Implementation(UAC_Inventory*& Component)
{
	II_Inventory::GetInventoryComponent_Implementation(Component);

	Component = InventoryComponent;
}

// void ADBCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
// {
// 	if (GetController() == nullptr)
// 	{
// 		if (HasAuthority())
// 		{
// 			const FGenericTeamId OldTeamID = MyTeamID;
// 			MyTeamID = NewTeamID;
// 			ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
// 		}
// 		else
// 		{
// 			UE_LOG(LogDBTeams, Error, TEXT("You can't set the team ID on a character (%s) except on the authority"), *GetPathNameSafe(this));
// 		}
// 	}
// 	else
// 	{
// 		UE_LOG(LogDBTeams, Error, TEXT("You can't set the team ID on a possessed character (%s); it's driven by the associated controller"), *GetPathNameSafe(this));
// 	}
// }

// FGenericTeamId ADBCharacter::GetGenericTeamId() const
// {
// 	return MyTeamID;
// }

// FOnDBTeamIndexChangedDelegate* ADBCharacter::GetOnTeamIndexChangedDelegate()
// {
// 	return &OnTeamChangedDelegate;
// }

// void ADBCharacter::FastSharedReplication_Implementation(const FSharedRepMovement& SharedRepMovement)
// {
// 	if (GetWorld()->IsPlayingReplay())
// 	{
// 		return;
// 	}
//
// 	// Timestamp is checked to reject old moves.
// 	if (GetLocalRole() == ROLE_SimulatedProxy)
// 	{
// 		// Timestamp
// 		ReplicatedServerLastTransformUpdateTimeStamp = SharedRepMovement.RepTimeStamp;
//
// 		// Movement mode
// 		if (ReplicatedMovementMode != SharedRepMovement.RepMovementMode)
// 		{
// 			ReplicatedMovementMode = SharedRepMovement.RepMovementMode;
// 			GetCharacterMovement()->bNetworkMovementModeChanged = true;
// 			GetCharacterMovement()->bNetworkUpdateReceived = true;
// 		}
//
// 		// Location, Rotation, Velocity, etc.
// 		FRepMovement& MutableRepMovement = GetReplicatedMovement_Mutable();
// 		MutableRepMovement = SharedRepMovement.RepMovement;
//
// 		// This also sets LastRepMovement
// 		OnRep_ReplicatedMovement();
//
// 		// Jump force
// 		bProxyIsJumpForceApplied = SharedRepMovement.bProxyIsJumpForceApplied;
//
// 		// Crouch
// 		if (bIsCrouched != SharedRepMovement.bIsCrouched)
// 		{
// 			bIsCrouched = SharedRepMovement.bIsCrouched;
// 			OnRep_IsCrouched();
// 		}
// 	}
// }

// bool ADBCharacter::UpdateSharedReplication()
// {
// 	if (GetLocalRole() == ROLE_Authority)
// 	{
// 		FSharedRepMovement SharedMovement;
// 		if (SharedMovement.FillForCharacter(this))
// 		{
// 			// Only call FastSharedReplication if data has changed since the last frame.
// 			// Skipping this call will cause replication to reuse the same bunch that we previously
// 			// produced, but not send it to clients that already received. (But a new client who has not received
// 			// it, will get it this frame)
// 			if (!SharedMovement.Equals(LastSharedReplication, this))
// 			{
// 				LastSharedReplication = SharedMovement;
// 				ReplicatedMovementMode = SharedMovement.RepMovementMode;
//
// 				FastSharedReplication(SharedMovement);
// 			}
// 			return true;
// 		}
// 	}
//
// 	// We cannot fastrep right now. Don't send anything.
// 	return false;
// }

void ADBCharacter::OnAbilitySystemInitialized()
{
	
}

void ADBCharacter::OnAbilitySystemUninitialized()
{
	
}

void ADBCharacter::PossessedBy(AController* NewController)
{
	// const FGenericTeamId OldTeamID = MyTeamID;

	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();

	// // Grab the current team ID and listen for future changes
	// if (IDBTeamAgentInterface* ControllerAsTeamProvider = Cast<IDBTeamAgentInterface>(NewController))
	// {
	// 	MyTeamID = ControllerAsTeamProvider->GetGenericTeamId();
	// 	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnControllerChangedTeam);
	// }
	// ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ADBCharacter::UnPossessed()
{
	AController* const OldController = Controller;

	// // Stop listening for changes from the old controller
	// const FGenericTeamId OldTeamID = MyTeamID;
	// if (IDBTeamAgentInterface* ControllerAsTeamProvider = Cast<IDBTeamAgentInterface>(OldController))
	// {
	// 	ControllerAsTeamProvider->GetTeamChangedDelegateChecked().RemoveAll(this);
	// }

	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();

	// // Determine what the new team ID should be afterwards
	// MyTeamID = DetermineNewTeamAfterPossessionEnds(OldTeamID);
	// ConditionalBroadcastTeamChanged(this, OldTeamID, MyTeamID);
}

void ADBCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	PawnExtComponent->HandleControllerChanged();
}

void ADBCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	PawnExtComponent->HandlePlayerStateReplicated();
}

void ADBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void ADBCharacter::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		for (const TPair<uint8, FGameplayTag>& TagMapping : DBGameplayTags::MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				DBASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : DBGameplayTags::CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				DBASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UPaperCharacterMovementComponent* DBMoveComp = CastChecked<UPaperCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(DBMoveComp->MovementMode, DBMoveComp->CustomMovementMode, true);
	}
}

void ADBCharacter::FellOutOfWorld(const class UDamageType& dmgType)
{
	Super::FellOutOfWorld(dmgType);
	// HealthComponent->DamageSelfDestruct(/*bFellOutOfWorld=*/ true);
}

void ADBCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ADBCharacter::OnDeathFinished(AActor* OwningActor)
{
	// TODO: Consider change to Lootable state.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ADBCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UPaperCharacterMovementComponent* DBMoveComp = CastChecked<UPaperCharacterMovementComponent>(GetCharacterMovement());
	DBMoveComp->StopMovementImmediately();
	DBMoveComp->DisableMovement();
}

void ADBCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ADBCharacter::UninitAndDestroy()
{
	// TODO: Consider change to Lootable state.
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	// Uninitialize the ASC if we're still the avatar actor (otherwise another pawn already did it when they became the avatar actor)
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		if (DBASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UninitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

void ADBCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UPaperCharacterMovementComponent* DBMoveComp = CastChecked<UPaperCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(DBMoveComp->MovementMode, DBMoveComp->CustomMovementMode, true);
}

void ADBCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = DBGameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = DBGameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			DBASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
}

// void ADBCharacter::ToggleCrouch()
// {
// 	const UPaperCharacterMovementComponent* DBMoveComp = CastChecked<UPaperCharacterMovementComponent>(GetCharacterMovement());
//
// 	if (bIsCrouched || DBMoveComp->bWantsToCrouch)
// 	{
// 		UnCrouch();
// 	}
// 	else if (DBMoveComp->IsMovingOnGround())
// 	{
// 		Crouch();
// 	}
// }

void ADBCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		DBASC->SetLooseGameplayTagCount(DBGameplayTags::Status_Crouching, 1);
	}


	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void ADBCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (UDBAbilitySystemComponent* DBASC = GetDBAbilitySystemComponent())
	{
		DBASC->SetLooseGameplayTagCount(DBGameplayTags::Status_Crouching, 0);
	}

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool ADBCharacter::CanJumpInternal_Implementation() const
{
	// same as ACharacter's implementation but without the crouch check
	return JumpIsAllowedInternal();
}

