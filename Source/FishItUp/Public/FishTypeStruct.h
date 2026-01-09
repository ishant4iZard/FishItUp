// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "FishTypeStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FFishType
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsShiny;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Image;

};

USTRUCT(BlueprintType)
struct FFishStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TimesCaught = 0;

    UPROPERTY(BlueprintReadOnly)
    float TotalWeight = 0.f;

    UPROPERTY(BlueprintReadOnly)
    float HighestWeight = 0.f;
};


USTRUCT(BlueprintType)
struct FFishGlossaryEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 FishTypeIndex;

    UPROPERTY(BlueprintReadOnly)
    bool bIsDiscovered;

    UPROPERTY(BlueprintReadOnly)
    bool bIsShiny;

    UPROPERTY(BlueprintReadOnly)
    int32 TimesCaught;

    UPROPERTY(BlueprintReadOnly)
    float HighestWeight;

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    UTexture2D* Image;
};