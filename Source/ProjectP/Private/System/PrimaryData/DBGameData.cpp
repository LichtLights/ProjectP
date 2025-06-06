// Fill out your copyright notice in the Description page of Project Settings.


#include "System/PrimaryData/DBGameData.h"

#include "System/ManagerClasses/DBAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DBGameData)

UDBGameData::UDBGameData()
{
}

const UDBGameData& UDBGameData::Get()
{
	return UDBAssetManager::Get().GetGameData();
}
