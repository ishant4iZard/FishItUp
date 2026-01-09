// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UFishingGameInstance::Init()
{
    Super::Init();

    if (UGameplayStatics::DoesSaveGameExist(TEXT("FishSave"), 0))
    {
        CurrentSaveGame = Cast<UFishSaveGame>(
            UGameplayStatics::LoadGameFromSlot(TEXT("FishSave"), 0)
        );
    }
    else
    {
        CurrentSaveGame = Cast<UFishSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UFishSaveGame::StaticClass())
        );
    }
}

void UFishingGameInstance::BuildFishGlossary(UFishSaveGame* SaveGame, TArray<FFishGlossaryEntry>& OutNormalEntries, TArray<FFishGlossaryEntry>& OutShinyEntries)
{
    OutNormalEntries.Empty();
    OutShinyEntries.Empty();

    for (int32 FishIndex = 0; FishIndex < FishTypeLibrary->NonShinyFish.Num(); FishIndex++)
    {
        FFishGlossaryEntry NormalEntry;
        FFishGlossaryEntry ShinyEntry;

        NormalEntry.FishTypeIndex = FishIndex;
        ShinyEntry.FishTypeIndex = FishIndex;

        const FFishType& NormalFishType = FishTypeLibrary->NonShinyFish[FishIndex];
        const FFishType& ShinyFishType = FishTypeLibrary->ShinyFish[FishIndex];

        NormalEntry.Name = NormalFishType.Name;
        NormalEntry.Image = NormalFishType.Image;

        ShinyEntry.Name = ShinyFishType.Name;
        ShinyEntry.Image = ShinyFishType.Image;

        const FFishStats* NormalStats =
            SaveGame->FishStatsMap.Find(FishIndex);

        const FFishStats* ShinyStats =
            SaveGame->ShinyFishStatsMap.Find(FishIndex);

        NormalEntry.TimesCaught =
            NormalStats ? NormalStats->TimesCaught : 0;

        ShinyEntry.TimesCaught =
            ShinyStats ? ShinyStats->TimesCaught : 0;

        NormalEntry.HighestWeight =
            NormalStats
            ? NormalStats->HighestWeight
            : 0.f;

        ShinyEntry.HighestWeight =
            ShinyStats
            ? ShinyStats->HighestWeight
            : 0.f;

        NormalEntry.bIsDiscovered =
            NormalEntry.TimesCaught > 0;

        ShinyEntry.bIsDiscovered =
            NormalEntry.TimesCaught > 0;


        OutNormalEntries.Add(NormalEntry);
        OutShinyEntries.Add(NormalEntry);
    }

}