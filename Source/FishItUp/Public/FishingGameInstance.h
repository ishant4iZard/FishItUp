// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FishTypeLibrary.h"
#include "FishSaveGame.h"

#include "FishingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FISHITUP_API UFishingGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFishTypeLibrary* FishTypeLibrary;

	UPROPERTY()
	UFishSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable)
	void BuildFishGlossary(UFishSaveGame* SaveGame, TArray<FFishGlossaryEntry>& OutNormalEntries, TArray<FFishGlossaryEntry>& OutShinyEntries);

protected:
	virtual void Init()override;
};
