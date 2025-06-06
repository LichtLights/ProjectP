// Fill out your copyright notice in the Description page of Project Settings.


#include "DB_IFPSystem/Items/DBItemActor.h"

#include "GlobalConstants.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "AbilitySystem/Components/DBAbilitySystemComponent.h"
#include "Characters/PlayerCharacters/DBPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Core/Data/FL_InventoryFramework.h"
#include "DB_IFPSystem/Items/DA_DBCoreItem.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADBItemActor::ADBItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);

	BoxComp->SetBoxExtent(FVector(16, 16, 16));
	BoxComp->SetCollisionProfileName(TEXT("InteractableDynamic"), true);
	BoxComp->SetEnableGravity(true);
	BoxComp->SetSimulatePhysics(true);
	// TODO: Should Lock after burst? burst doesn't work when it's locked from the start.
	// BoxComp->GetBodyInstance()->bLockXTranslation = true;
	// BoxComp->GetBodyInstance()->bLockYTranslation = true;
	BoxComp->SetNotifyRigidBodyCollision(true);
	InitialBoxRotation = BoxComp->GetRelativeRotation();

	BoxComp->OnComponentHit.AddDynamic(this, &ADBItemActor::OnBoxComponentHit);

	// TODO: DEBUG
	BoxComp->SetHiddenInGame(false);

	ItemSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Item Sprite"));
	ItemSprite->SetupAttachment(BoxComp);
	ItemSprite->SetRelativeRotation(GlobalConstants::SpriteRotatorConstantRight);
	ItemSprite->SetCollisionProfileName(TEXT("NoCollision"));
	ItemSprite->SetCastShadow(true);
}

// Called when the game starts or when spawned
void ADBItemActor::BeginPlay()
{
	Super::BeginPlay();

	InitializeDBItemActor();
}

// Called every frame
void ADBItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCanBeInteracted();	
}

void ADBItemActor::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& OptionBuilder)
{
	// TODO: Gather available interaction options related to current item that item actor holds.

	// TODO: DEBUG
	OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Unknown, this));
	
	OptionBuilder.AddInteractionOption(FInteractionOption::CreateInteractionOption(EInteractionOptionType::Collect, this));
}

void ADBItemActor::CustomizeInteractionEventData(const FGameplayTag& InteractionEventTag,
	FGameplayEventData& InOutEventData)
{
	IInteractableTarget::CustomizeInteractionEventData(InteractionEventTag, InOutEventData);
}

bool ADBItemActor::IsInteractionReady()
{
	return bCanBeInteracted;
}

// void ADBItemActor::ReadyInteraction(AActor* PlayerCharacter)
// {
// 	IInteractableTarget::ReadyInteraction(PlayerCharacter);
//
// 	// TODO: Implement this with Interaction GameplayAbility.
// 	auto DBCharacter = Cast<ADBPlayerCharacter>(PlayerCharacter);
// 	if (!IsValid(DBCharacter)) return;
//
// 	auto AbilityComponent = DBCharacter->GetDBAbilitySystemComponent();
// 	if (!IsValid(AbilityComponent)) return;
//
// 	
// 	UAC_Inventory* PlayerInventoryComponent = PlayerCharacter->FindComponentByClass<UAC_Inventory>();
// 	
// 	if (!IsValid(PlayerInventoryComponent)) return;
//
// 	if (UFL_InventoryFramework::IsUniqueIDValid(RootItemID))
// 	{
// 		PlayerInventoryComponent->C_RequestServerDataFromOtherComponent(InventoryComponent, false);
// 	}
// }
//
// void ADBItemActor::TriggerInteraction(AActor* PlayerCharacter)
// {
// 	IInteractableTarget::TriggerInteraction(PlayerCharacter);
//
// 	UAC_Inventory* PlayerInventoryComponent = PlayerCharacter->FindComponentByClass<UAC_Inventory>();
//
// 	if (!IsValid(PlayerInventoryComponent)) return;
//
// 	if (UFL_InventoryFramework::IsUniqueIDValid(RootItemID))
// 	{
// 		PlayerInventoryComponent->C_RequestServerDataFromOtherComponent(InventoryComponent, true);
// 	}
// 	
// }
//
// void ADBItemActor::EndInteraction(AActor* PlayerCharacter)
// {
// 	IInteractableTarget::EndInteraction(PlayerCharacter);
//
// 	UAC_Inventory* PlayerInventoryComponent = PlayerCharacter->FindComponentByClass<UAC_Inventory>();
//
// 	if (!IsValid(PlayerInventoryComponent)) return;
//
// 	InventoryComponent->S_RemoveListener(PlayerInventoryComponent);
// }

void ADBItemActor::UpdateCanBeInteracted()
{
	if (bIsSpinning)
	{
		bCanBeInteracted = false;
	}
	else
	{
		bCanBeInteracted = true;
	}	

}

void ADBItemActor::InitializeDBItemActor()
{
	// TODO: How to setup? the item image?
	if (UDA_CoreItem* BaseItem = GetRootItemData().ItemAssetSoftReference.LoadSynchronous())
	// if (UDA_CoreItem* BaseItem = GetRootItemData().ItemAsset)
	{
		if (const UDA_DBCoreItem* DBItem = Cast<UDA_DBCoreItem>(BaseItem))
		{
			if (DBItem->ItemSprite)
				ItemSprite->SetSprite(DBItem->ItemSprite);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The loaded asset is not of type UDA_DBCoreItem."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The loaded asset is null."));
	}

	LaunchAndSpin(false);	
}

void ADBItemActor::LaunchAndSpin(bool bIsBurst)
{
	if (!BoxComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BoxComp is null at ADBItemActor::LaunchAndSpin()"));
		return;
	}

	bIsSpinning = true;

	if (bIsBurst)
	{
		// When first dropped with burst
		
		// Apply an impulse to launch the actor
		FVector LaunchDirection = FVector(FMath::FRandRange(-0.3f, 0.3f), FMath::FRandRange(-0.3f, 0.3f), 1); // Randomize horizontal direction
		LaunchDirection.Normalize();
		float LaunchStrength = FMath::FRandRange(400.0f, 600.0f); // Randomize launch strength
		BoxComp->AddImpulse(LaunchDirection * LaunchStrength, NAME_None, true);
	}
	else
	{
		// When only one(current) item
		
		// Apply an impulse to launch the actor
		FVector LaunchDirection = FVector(0, 0, 1); // Upward direction
		float LaunchStrength = 500.0f; // Strength of the launch
		BoxComp->AddImpulse(LaunchDirection * LaunchStrength, NAME_None, true);
	}

	// Set a timer to apply a continuous spin
	float SpinInterval = 0.01f; // Interval for the spin application
	GetWorld()->GetTimerManager().SetTimer(
		SpinTimerHandle,
		this,
		&ADBItemActor::ApplySpin,
		SpinInterval,
		true
	);
}

void ADBItemActor::ApplySpin() const
{
	if (!BoxComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BoxComp is null at ADBItemActor::ApplySpin()"));
		return;
	}

	// Apply a torque to spin the actor
	const FVector SpinTorque = FVector(0, 0, 1500.0f); // Z-axis torque for spin
	BoxComp->AddTorqueInDegrees(SpinTorque, NAME_None, true);
}

void ADBItemActor::StopSpinAndReset()
{
	// Clear the spin timer
	GetWorld()->GetTimerManager().ClearTimer(SpinTimerHandle);

	BoxComp->SetRelativeRotation(InitialBoxRotation);
	bIsSpinning = false;
}

void ADBItemActor::OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopSpinAndReset();
}
