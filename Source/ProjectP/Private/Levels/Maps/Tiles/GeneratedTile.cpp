// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/Maps/Tiles/GeneratedTile.h"

#include "GlobalConstants.h"
#include "PaperTileMapComponent.h"


// Sets default values
AGeneratedTile::AGeneratedTile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Tile Map Component"));
	SetRootComponent(TileMapComponent);

	TileMapComponent->SetWorldRotation(GlobalConstants::TileMapRotatorConstant);
}

// Called when the game starts or when spawned
void AGeneratedTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGeneratedTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

