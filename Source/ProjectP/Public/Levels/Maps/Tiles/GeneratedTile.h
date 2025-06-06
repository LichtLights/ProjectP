// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratedTile.generated.h"

class UPaperTileMapComponent;
class UPaperSprite;

UENUM(BlueprintType)
enum class ETileConnectionInfo : uint8
{
	Open = 0,
	Closed = 1
};

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Floor = 0,
	BlockedWithDooDads = 1,
	
	Fall = 1,
	Water = 2
};

USTRUCT(BlueprintType)
struct FTileConnection
{
	GENERATED_BODY()

	// This represents the type of connection on each side of the tile (Top, Bottom, Left, Right)
	// For simplicity, we can use an ID or name to represent the connection type
	UPROPERTY(BlueprintReadWrite)
	ETileConnectionInfo TopConnection;
    
	UPROPERTY(BlueprintReadWrite)
	ETileConnectionInfo BottomConnection;
    
	UPROPERTY(BlueprintReadWrite)
	ETileConnectionInfo LeftConnection;
    
	UPROPERTY(BlueprintReadWrite)
	ETileConnectionInfo RightConnection;
	
};

UCLASS()
class PROJECTP_API AGeneratedTile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGeneratedTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	FTileConnection TileConnections;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	UPaperTileMapComponent* TileMapComponent;

	// Optionally, you can store a reference to a Paper2D sprite or other visual representation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	UPaperSprite* AdditionalTileSprite;

	

	// Tile type (ground, wall, doorway, etc.)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Data")
	ETileType TileType;

	// Function to determine if this tile is eligible for a certain doodad (for later use)
	bool IsEligibleForDoodad() const
	{
		return TileType == ETileType::Floor || TileType == ETileType::BlockedWithDooDads;
	}
	
};
