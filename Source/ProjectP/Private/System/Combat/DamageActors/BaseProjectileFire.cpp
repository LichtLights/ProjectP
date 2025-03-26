// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Combat/DamageActors/BaseProjectileFire.h"

#include "GlobalConstants.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ABaseProjectileFire::ABaseProjectileFire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);

	// BoxComp->SetCollisionProfileName("Trigger");

	// BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseMeleeSlash::OnSlashOverlap);

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	
	Sprite->CastShadow = true;
	Sprite->SetRelativeRotation(GlobalConstants::SlashRotatorConstant);
	Sprite->SetLooping(true);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComp->UpdatedComponent = BoxComp;
}

// Called when the game starts or when spawned
void ABaseProjectileFire::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectileFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectileFire::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	
}

