// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacters/DBPlayerCharacter.h"

#include "GlobalConstants.h"
#include "InputActionValue.h"
#include "PaperFlipbookComponent.h"
#include "PaperZDAnimInstance.h"
#include "AbilitySystem/DBAbilitySet.h"
#include "AbilitySystem/AttributeSets/DBStatusAttributeSet.h"
#include "AbilitySystem/Components/DBWeaponMasteryComponent.h"
#include "Characters/Components/DBHeroComponent.h"
#include "Characters/Components/PaperCharacterMovementComponent.h"
#include "Characters/PlayerCharacters/CharacterComponents/PlayerAbilitySystemComponent.h"
#include "Characters/PlayerCharacters/DAs/DA_OverridingAnims.h"
#include "Components/CapsuleComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LogChannel/DBLogChannels.h"
#include "System/Camera/DBCameraComponent.h"
#include "System/Combat/CombatManager.h"
#include "System/GameModes/DBExperienceManagerComponent.h"
#include "System/GameModes/DBGameMode.h"
#include "System/Player/PlayerState/DBPlayerState.h"
#include "System/PlayerController/DBPlayerController.h"
#include "System/PrimaryData/DBPawnData.h"

// Sets default values
ADBPlayerCharacter::ADBPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UPaperCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)
	)
{
	// Camera related settings
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetRelativeRotation(GlobalConstants::CameraRotatorConstant);
	SpringArmComp->SetUsingAbsoluteRotation(true);
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->TargetArmLength = 580.0f;
    	
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;
    	
	CameraComp = CreateDefaultSubobject<UDBCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
    
	// Create and initialize the interaction sphere component
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Sphere"));
	InteractionSphere->SetupAttachment(RootComponent);
	InteractionSphere->SetSphereRadius(InteractionRange);
	InteractionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap); // Use custom channel, ChannelName = Interaction
    
	// Create and initialize the interaction widget component
	InteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Widget"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidgetComponent->SetDrawAtDesiredSize(true);
	
	// Components
	AbilitySystemComp = ObjectInitializer.CreateDefaultSubobject<UPlayerAbilitySystemComponent>(this, TEXT("Ability System Component"));
	// AbilitySystemComp->SetIsReplicated(true);
	// AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	// StatusAttributeSet = CreateDefaultSubobject<UDBStatusAttributeSet>(TEXT("StatusAttributeSet"));
	// CombatSet = CreateDefaultSubobject<UDBCombatSet>(TEXT("CombatSet"));

	HeroComponent = CreateDefaultSubobject<UDBHeroComponent>(TEXT("Hero Component"));

	WeaponMasteryComp = CreateDefaultSubobject<UDBWeaponMasteryComponent>(TEXT("Weapon Mastery Component"));
}

// Called when the game starts or when spawned
void ADBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initially hide the widget
	InteractionWidgetComponent->SetVisibility(false);
}

// Called to bind functionality to input
void ADBPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADBPlayerCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ADBPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComp);
	
	// Initialize ability actor info
	if (IsValid(AbilitySystemComp))
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
		StatusAttributeSet = AbilitySystemComp->GetSet<UDBStatusAttributeSet>();
	}

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		UDBExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UDBExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnDBExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}

	if (IsValid(WeaponMasteryComp))
	{
		WeaponMasteryComp->GrantUnlockedAbilities();
	}
}

void ADBPlayerCharacter::SetPawnData(const UDBPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogDB, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	for (const UDBAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComp, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, ADBPlayerState::NAME_DBAbilityReady);
	
	ForceNetUpdate();
}

void ADBPlayerCharacter::OnExperienceLoaded(const UDBExperienceDefinition* UDBExperienceDefinition)
{
	if (ADBGameMode* DBGameMode = GetWorld()->GetAuthGameMode<ADBGameMode>())
	{
		if (const UDBPawnData* NewPawnData = DBGameMode->GetPawnDataForController(GetController()))
		{
			
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogDB, Error, TEXT("ADBPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void ADBPlayerCharacter::Interact()
{
	if (CurrentInteractingInteractable)
	{
		// IInteractable* Interactable = Cast<IInteractable>(CurrentInteractingInteractable);
		//
		// if (Interactable)
		// {
		// 	const auto PlayerController = Cast<ADBPlayerController>(GetController());
		// 	const auto InteractionWidget = PlayerController->GetCurrentOpenedInteractionWidget();
		// 	if (!InteractionWidget)
		// 	{
		// 		return;				
		// 	}
		// 	const EInteractionClass CurrentInteractionClass = InteractionWidget->GetCurrentInteractionClass();
		// 	
		// 	Interactable->Interact(this, CurrentInteractionClass);
		// 	UpdateAvailableInteraction();
		// }
	}
}

FVector ADBPlayerCharacter::GetAimAtMouse()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		FVector2D MousePosition;
		if (PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y))
		{
			FVector WorldLocation, WorldDirection;
			if (UGameplayStatics::DeprojectScreenToWorld(PlayerController, MousePosition, WorldLocation, WorldDirection))
			{
				// Trace to find where the mouse is pointing in the world
				FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);

				FHitResult HitResult;
				GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility);

				if (HitResult.bBlockingHit)
				{
					FVector AimLocation = HitResult.Location;
					// FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AimLocation);
					// SetActorRotation(FRotator(0.0f, AimRotation.Yaw, 0.0f));

					RecentAimLocation = AimLocation;
					return AimLocation;
				}
				
			}
			
		}
		
	}

	return RecentAimLocation;
}

void ADBPlayerCharacter::PrimaryAttack()
{
	// if (bIsAttacking || bIsStunned)
	// {
	// 	return;
	// }
	
	// bIsAttacking = true;

	// if (!StatusComp->bCanAttack || !StatusComp->bCanPrimaryAttack)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Can't Attack right now"));
	// 	return;
	// }

	// StatusComp->bCanMove = false;

	auto TargetLocation = GetAimAtMouse();
	// UE_LOG(LogTemp, Warning, TEXT("TargetLocation: %s"), *TargetLocation.ToString());


	// Set the Sprite facing to the TargetLocation
	FVector CharacterLocation = GetActorLocation();

	// Determine the direction to face (left or right)
	if (TargetLocation.X < CharacterLocation.X)
	{
		// Target is to the left, so face left
		GetController()->SetControlRotation(FRotator(0, 180.f, 0));
		GetSprite()->SetRelativeRotation(FRotator(0, 0, 20));
	}
	else
	{
		// Target is to the right, so face right
		GetController()->SetControlRotation(FRotator(0, 0, 0));
		GetSprite()->SetRelativeRotation(FRotator(0, 0, -20));
	}
	
	// TODO: Implement Area HitBox attack using TargetLocation.
	UCombatManager::MeleeSlash(
		GetWorld(),
		this,
		EMeleeSlashType::Test,
		CharacterLocation,
		GetActorRotation(),
		TargetLocation,
		FRotator(0, 0, 0)
	);	

	// TODO: Differentiate animation between Weapon types.
	// TODO: Combo Attacks. via abilities.
	// TODO: Attack speed should be affect the speed of the animation.
	// Play attack animation
	FName SlotName = "DefaultSlot";
	GetAnimInstance()->PlayAnimationOverride(DA_OverridingAnims->AS_PrimaryAttack, SlotName);

	// TODO: Implement via FZDOnAnimationOverrideEndSignature, not TimerHandle.
	auto AnimDuration = DA_OverridingAnims->AS_PrimaryAttack->GetTotalDuration();
	
	GetWorld()->GetTimerManager().SetTimer(
		PrimaryAttackTimerHandle,
		this,
		&ADBPlayerCharacter::AttackAnimTimerExpired,
		AnimDuration,
		false
	);
}

void ADBPlayerCharacter::AttackAnimTimerExpired()
{
	// StatusComp->bCanMove = true;
}

void ADBPlayerCharacter::SwitchCurrentInteractable()
{
	if (InteractablesCanInteractWith.IsEmpty() || InteractablesCanInteractWith.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractablesCanInteractWith is empty or only 1"));
		return;
	}

	CurrentInteractableIndex = InteractablesCanInteractWith.IndexOfByKey(CurrentInteractingInteractable);
	if (CurrentInteractableIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("current interactable index is NONE"));
		return;
	}

	auto NextInteractableIndex = CurrentInteractableIndex + 1;

	if (NextInteractableIndex >= InteractablesCanInteractWith.Num())
	{
		NextInteractableIndex = 0;
		if (NextInteractableIndex == CurrentInteractableIndex)
		{
			UE_LOG(LogTemp, Warning, TEXT("Next Interactable Index is equal to Current Interactable Index"));
			return;
		}
	}
	AActor* OtherInteractable = InteractablesCanInteractWith[NextInteractableIndex];

	UE_LOG(LogTemp, Warning, TEXT("NextInteractableIndex: %d"), NextInteractableIndex);
	UpdateInteractionWidget(OtherInteractable);
	UE_LOG(LogTemp, Warning, TEXT("UpdateInteractionWidget: %s"), *OtherInteractable->GetName());
}

void ADBPlayerCharacter::UpdateInteractionWidget(AActor* Interactable)
{
	// if (Interactable)
	// {
	// 	InteractionWidgetComponent->SetVisibility(true);
	//
	// 	if (UUserWidget* Widget = Cast<UUserWidget>(InteractionWidgetComponent->GetWidget()))
	// 	{
	// 		auto InteractionInfo = Cast<IInteractable>(Interactable)->GetAvailableInteractions();
	// 		
	// 		auto InteractionWidget = Cast<UInteractionWidget>(InteractionWidgetComponent->GetWidget());
	// 		if (!InteractionWidget)
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("InteractionWidget(local) is null at ABasePlayerCharacter::UpdateInteractionWidget()"));
	// 			return;
	// 		}
	// 		// Sets available interactions
	// 		InteractionWidget->SetAvailableInteractions(InteractionInfo);
	// 		
	// 		// Get address for InteractionWidget
	// 		Cast<ADBPlayerController>(GetController())->SetCurrentOpenedInteractionWidget(InteractionWidget);
	//
	// 		// Re-assign currently interacting interactable actor
	// 		CurrentInteractingInteractable = Interactable;
	//
	//
	// 		// Attach widget component to interactable
	// 		const auto AttachComp = Cast<IInteractable>(Interactable)->GetInteractionWidgetAttachComponent();
	// 		if (!AttachComp)
	// 		{
	// 			UE_LOG(LogTemp, Warning, TEXT("AttachComp is null at ABasePlayerCharacter::UpdateInteractionWidget(AActor* Interactable)"));
	// 			return;
	// 		}			
	// 		InteractionWidgetComponent->AttachToComponent(
	// 			AttachComp,
	// 			FAttachmentTransformRules::SnapToTargetNotIncludingScale
	// 		);
	// 		// const FVector2D DrawSize = InteractionWidgetComponent->GetCurrentDrawSize();
	// 		// InteractionWidgetComponent->AddRelativeLocation(FVector(0, 0, DrawSize.Y / 2.0f));
	//
	// 	}
	// }
	// else
	// {
	// 	if (UUserWidget* Widget = Cast<UUserWidget>(InteractionWidgetComponent->GetWidget()))
	// 	{
	// 		InteractionWidgetComponent->AttachToComponent(
	// 				this->RootComponent,
	// 				FAttachmentTransformRules::SnapToTargetNotIncludingScale
	// 			);
	// 	}
	// 	
	// 	InteractionWidgetComponent->SetVisibility(false);
	// }
}

UAbilitySystemComponent* ADBPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
