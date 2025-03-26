// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PrimaryData/DBPawnData.h"

UDBPawnData::UDBPawnData(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PawnClass = nullptr;
	InputConfig = nullptr;
	// DefaultCameraMode = nullptr;
}
