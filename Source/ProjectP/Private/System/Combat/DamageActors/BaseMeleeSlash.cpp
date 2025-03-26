// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Combat/DamageActors/BaseMeleeSlash.h"

#include "GlobalConstants.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ABaseMeleeSlash::ABaseMeleeSlash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);

	// BoxComp->SetCollisionProfileName("Trigger");

	// BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseMeleeSlash::OnSlashOverlap);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	
	Sprite->CastShadow = false;
	Sprite->SetRelativeRotation(GlobalConstants::SlashRotatorConstant);
	Sprite->SetLooping(false);
}

// Called when the game starts or when spawned
void ABaseMeleeSlash::BeginPlay()
{
	Super::BeginPlay();

	Sprite->OnFinishedPlaying.AddDynamic(this, &ABaseMeleeSlash::AnimationEnded);
}

// Called every frame
void ABaseMeleeSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseMeleeSlash::AnimationEnded()
{
	Destroy();
}

void ABaseMeleeSlash::OnSlashOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
