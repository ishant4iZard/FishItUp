// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FishingQTEWidget.generated.h"

/**
 * 
 */

class UCanvasPanel;
class UImage;
class UMaterialInterface;

UENUM(BlueprintType)
enum class EQTEDirection : uint8
{
    None,
    Up,
    Down,
    Left,
    Right
};

USTRUCT(BlueprintType)
struct FQTESlice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngleStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AngleWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQTEDirection Direction;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTESliceSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQTESuccess,int32, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQTEFail);

UCLASS()
class FISHITUP_API UFishingQTEWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void BuildSlices(const TArray<FQTESlice>& Slices);

    bool bQTEFailed = false;

    UPROPERTY(BlueprintAssignable)
    FOnQTESliceSuccess OnQTESliceSuccess;

    UPROPERTY(BlueprintAssignable)
    FOnQTESuccess OnQTESuccess;
    
    UPROPERTY(BlueprintAssignable)
    FOnQTEFail OnQTEFail;



protected:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UCanvasPanel* RingCanvas;

    UPROPERTY(EditAnywhere, Category = "QTE")
    UMaterialInterface* RingMaterial;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UImage* ArrowImage;

    UPROPERTY()
    TArray<UImage*> SliceImages;

    UPROPERTY()
    TArray<UMaterialInstanceDynamic*> SliceMIDs;

    UPROPERTY(BlueprintReadOnly)
    TArray<FQTESlice> QTESlices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArrowRotationSpeed = 45.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpeedMult = 1.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DefaultArrowRotationSpeed = 45.f;

    float CurrentArrowAngle = 0.f;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    int32 CurrentSliceIndex = 0;

    float ThicknessPulse = 0.f;
    float PulseSpeed = 6.f;


    float AccumulatedAccuracy = 0.f;
    int32 SuccessfulSlices = 0;
    bool bPerfectRun = true;

    UPROPERTY(EditDefaultsOnly, Category = "QTE|Scoring")
    float MaxScore = 100000.f;

public:
    UFUNCTION(BlueprintCallable)
    TArray<FQTESlice> GenerateQTESlices();

    UFUNCTION(BlueprintCallable)
    void HandleQTEInput(EQTEDirection InputDir);

protected:
    void GenerateSliceSizes(int32 NumSlices, TArray<float>& OutSizes) const;
    void DistributeGaps(int32 NumSlices, float RemainingAngle, TArray<float>& OutGaps) const;
    void BuildSlicesFromSizes(const TArray<float>& SliceSizes, const TArray<float>& Gaps);

    EQTEDirection GetRandomDirection() const;

    void RotateArrow(float DeltaTime);
    void ResetArrowRotationSpeed();

    bool IsAngleInSlice(float Angle, const FQTESlice& Slice) const;


    void OnSliceSuccess();

    void OnQTESuccessInternal();

    void OnQTEFailInternal();

    void PlaySliceSuccessFeedback(int32 SliceIndex);

    void PlayFailFeedback(int32 SliceIndex);

    float ComputeSliceAccuracy(float ArrowAngle, const FQTESlice& Slice) const;

    float ComputeDifficulty() const;

    int32 ComputeFinalScore() const;
};
