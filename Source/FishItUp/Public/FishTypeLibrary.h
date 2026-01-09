// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FishTypeStruct.h"

#include "FishTypeLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FISHITUP_API UFishTypeLibrary : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FFishType> NonShinyFish;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FFishType> ShinyFish;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FFishType> Garbage;
};
