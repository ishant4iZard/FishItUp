// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FishTypeStruct.h"

#include "FishSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FISHITUP_API UFishSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Key = FishTypeIndex (0–9)
	UPROPERTY()
	TMap<int32, FFishStats> FishStatsMap;

	UPROPERTY()
	TMap<int32, FFishStats> ShinyFishStatsMap;
};
