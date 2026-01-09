// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingQTEWidget.h"
#include "FishingQTEWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"


void UFishingQTEWidget::BuildSlices(const TArray<FQTESlice>& Slices)
{
    if (!RingCanvas || !RingMaterial) return;

    // Clear previous slices
    RingCanvas->ClearChildren();
    SliceImages.Empty();
    SliceMIDs.Empty();

    //reset 
    CurrentArrowAngle = 0;
    CurrentSliceIndex = 0;
    bQTEFailed = false;
    ArrowRotationSpeed = 0.f;
    AccumulatedAccuracy = 0.f;
    SuccessfulSlices = 0;
    bPerfectRun = true;

    FTimerHandle ArrowRotationTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        ArrowRotationTimerHandle,
        this,
        &UFishingQTEWidget::ResetArrowRotationSpeed,
        1.5f, 
        false
    );

    {
        // Create Image
        UImage* SliceImage = NewObject<UImage>(this);
        SliceImage->SetBrushFromMaterial(RingMaterial);

        // Add to canvas
        UCanvasPanelSlot* CanvasSlot = RingCanvas->AddChildToCanvas(SliceImage);
        CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
        CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

        CanvasSlot->SetPosition(FVector2D(0.f, 0.f));

        CanvasSlot->SetSize(FVector2D(300.f, 300.f));

        // Create MID
        UMaterialInstanceDynamic* MID =
            UMaterialInstanceDynamic::Create(RingMaterial, this);

        SliceImage->SetBrushFromMaterial(MID);

        // Set parameters
        MID->SetScalarParameterValue("AngleStart", 0);
        MID->SetScalarParameterValue("AngleWidth", 360);
        MID->SetVectorParameterValue("RingColor", FColor(128, 128, 128));

        SliceImages.Add(SliceImage);
        SliceMIDs.Add(MID);
    }

    for (int32 i = 0; i < Slices.Num(); i++)
    {
        // Create Image
        UImage* SliceImage = NewObject<UImage>(this);
        SliceImage->SetBrushFromMaterial(RingMaterial);

        // Add to canvas
        UCanvasPanelSlot* CanvasSlot = RingCanvas->AddChildToCanvas(SliceImage);
        CanvasSlot->SetAnchors(FAnchors(0.5f, 0.5f));
        CanvasSlot->SetAlignment(FVector2D(0.5f, 0.5f));

        CanvasSlot->SetPosition(FVector2D(0.f, 0.f));

        CanvasSlot->SetSize(FVector2D(300.f, 300.f));

        // Create MID
        UMaterialInstanceDynamic* MID =
            UMaterialInstanceDynamic::Create(RingMaterial, this);

        SliceImage->SetBrushFromMaterial(MID);

        // Set parameters
        MID->SetScalarParameterValue("AngleStart", Slices[i].AngleStart);
        MID->SetScalarParameterValue("AngleWidth", Slices[i].AngleWidth);



        FColor color;

        switch (Slices[i].Direction)
        {
        case EQTEDirection::None: {
            color = FColor(128, 128, 128);
            break;
        }
        case EQTEDirection::Up: {
            color = FColor::Red;
            break;
        }
        case EQTEDirection::Right: {
            color = FColor::Yellow;
            break;
        }
        case EQTEDirection::Down: {
            color = FColor::Green;
            break;
        }
        case EQTEDirection::Left: {
            color = FColor::Blue;
            break;
        }
        }

        MID->SetVectorParameterValue("RingColor", color);

        SliceImages.Add(SliceImage);
        SliceMIDs.Add(MID);
    }
}

void UFishingQTEWidget::NativeConstruct()
{
    Super::NativeConstruct();

    
}

void UFishingQTEWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    RotateArrow(InDeltaTime);

    if (ThicknessPulse > 0.f)
    {
        ThicknessPulse = FMath::FInterpTo(
            ThicknessPulse,
            0.f,
            InDeltaTime,
            PulseSpeed
        );

        SliceMIDs[CurrentSliceIndex]->SetScalarParameterValue("ThicknessPulse", ThicknessPulse);
    }
}

TArray<FQTESlice> UFishingQTEWidget::GenerateQTESlices()
{
    QTESlices.Empty();

    int32 NumSlices = FMath::RandRange(3, 5);


    TArray<float> SliceSizes;
    float TotalSize = 0.f;
    int count = 0;

    do {
        count++;
        GenerateSliceSizes(NumSlices, SliceSizes);

        TotalSize = 0;

        for (float Size : SliceSizes)
        {
            TotalSize += Size;
        }

        if (count > 5 && TotalSize > 330) {
            NumSlices--;
        }

    } while (TotalSize > 330);

    float RemainingAngle = 360.f - TotalSize;

    TArray<float> Gaps;
    DistributeGaps(NumSlices, RemainingAngle, Gaps);

    BuildSlicesFromSizes(SliceSizes, Gaps);

    SpeedMult = FMath::FRandRange(0.7f, 2.f);

    return QTESlices;
}

void UFishingQTEWidget::GenerateSliceSizes(int32 NumSlices, TArray<float>& OutSizes) const
{
    static const TArray<float> PossibleSizes = { 20.f, 30.f, 40.f };

    OutSizes.Empty();

    for (int32 i = 0; i < NumSlices; i++)
    {
        OutSizes.Add(PossibleSizes[FMath::RandRange(0, PossibleSizes.Num() - 1)]);
    }

}

void UFishingQTEWidget::DistributeGaps(int32 NumSlices, float RemainingAngle, TArray<float>& OutGaps) const
{
    OutGaps.Empty();

    float GapSum = 45.f;

    for (int32 i = 0; i < NumSlices + 1; i++)
    {
        float Gap = FMath::FRandRange(5.f, 20.f);
        OutGaps.Add(Gap);
        GapSum += Gap;
    }

    float Scale = RemainingAngle / GapSum;
    for (float& Gap : OutGaps)
    {
        Gap *= Scale;
    }
}

void UFishingQTEWidget::BuildSlicesFromSizes(const TArray<float>& SliceSizes, const TArray<float>& Gaps)
{
    float CurrentAngle = 45;

    for (int32 i = 0; i < SliceSizes.Num(); i++)
    {
        FQTESlice Slice;
        Slice.AngleStart = CurrentAngle;
        Slice.AngleWidth = SliceSizes[i];
        Slice.Direction = GetRandomDirection();

        QTESlices.Add(Slice);

        CurrentAngle += SliceSizes[i] + Gaps[i];
    }
}

EQTEDirection UFishingQTEWidget::GetRandomDirection() const
{
    return static_cast<EQTEDirection>(FMath::RandRange(1, 4));
}

void UFishingQTEWidget::RotateArrow(float DeltaTime)
{
    if (!ArrowImage)
        return;

    CurrentArrowAngle += ArrowRotationSpeed * DeltaTime;
    CurrentArrowAngle = FMath::Fmod(CurrentArrowAngle, 360.f);

    FWidgetTransform Transform;
    Transform.Angle = CurrentArrowAngle;

    ArrowImage->SetRenderTransform(Transform);
}

void UFishingQTEWidget::ResetArrowRotationSpeed()
{
    ArrowRotationSpeed = DefaultArrowRotationSpeed;
}

bool UFishingQTEWidget::IsAngleInSlice(float Angle, const FQTESlice& Slice) const
{
    float SliceStart = Slice.AngleStart;
    float SliceEnd = Slice.AngleStart + Slice.AngleWidth;

    // Normalize
    Angle = FMath::Fmod(Angle + 360.f, 360.f);

    if (SliceEnd <= 360.f)
    {
        return Angle >= SliceStart && Angle <= SliceEnd;
    }
    else
    {
        // Wrap-around slice
        return Angle >= SliceStart || Angle <= FMath::Fmod(SliceEnd, 360.f);
    }
}

void UFishingQTEWidget::HandleQTEInput(EQTEDirection InputDir)
{
    if (bQTEFailed || !QTESlices.IsValidIndex(CurrentSliceIndex) || ArrowRotationSpeed <0.1f)
        return;

    UE_LOG(LogTemp, Display, TEXT("QTEInput getting"));

    const FQTESlice& Slice = QTESlices[CurrentSliceIndex];

    bool bCorrectAngle = IsAngleInSlice(CurrentArrowAngle, Slice);
    bool bCorrectInput = (InputDir == Slice.Direction);

    if (bCorrectAngle && bCorrectInput)
    {
        OnSliceSuccess();
    }
    else
    {
        OnQTEFailInternal();
    }
}

void UFishingQTEWidget::OnSliceSuccess()
{
    const FQTESlice& Slice = QTESlices[CurrentSliceIndex];

    float Accuracy =
        ComputeSliceAccuracy(CurrentArrowAngle, Slice);

    AccumulatedAccuracy += Accuracy;
    SuccessfulSlices++;

    if (Accuracy < 0.95f)
    {
        bPerfectRun = false;
    }

    PlaySliceSuccessFeedback(CurrentSliceIndex);

    CurrentSliceIndex++;

    OnQTESliceSuccess.Broadcast();

    if (CurrentSliceIndex >= QTESlices.Num())
    {
        OnQTESuccessInternal();
    }
}

void UFishingQTEWidget::OnQTESuccessInternal()
{
    int32 FinalScore = ComputeFinalScore();

    OnQTESuccess.Broadcast(FinalScore);
}

void UFishingQTEWidget::OnQTEFailInternal()
{
    bQTEFailed = true;

    PlayFailFeedback(CurrentSliceIndex);

    ArrowRotationSpeed = 0.f;

    OnQTEFail.Broadcast();
}

void UFishingQTEWidget::PlaySliceSuccessFeedback(int32 SliceIndex)
{
    ThicknessPulse = 0.03f;

    // Example: set scalar on material
    SliceMIDs[SliceIndex+1]->SetScalarParameterValue("ThicknessPulse", ThicknessPulse);
}

void UFishingQTEWidget::PlayFailFeedback(int32 SliceIndex)
{
    SliceMIDs[SliceIndex+1]->SetScalarParameterValue("Saturation", 0.5f);
}

float UFishingQTEWidget::ComputeSliceAccuracy(float ArrowAngle, const FQTESlice& Slice) const
{
    float SliceCenter =
        Slice.AngleStart + (Slice.AngleWidth * 0.5f);

    float Delta =
        FMath::Abs(
            FMath::FindDeltaAngleDegrees(ArrowAngle, SliceCenter)
        );

    float HalfWidth = Slice.AngleWidth * 0.5f;

    return FMath::Clamp(
        1.f - (Delta / HalfWidth),
        0.f,
        1.f
    );
}

float UFishingQTEWidget::ComputeDifficulty() const
{
    float AvgSliceSize = 0.f;
    for (const FQTESlice& Slice : QTESlices)
    {
        AvgSliceSize += Slice.AngleWidth;
    }
    AvgSliceSize /= QTESlices.Num();

    float SliceWidthFactor =
        FMath::GetMappedRangeValueClamped(
            FVector2D(20.f, 40.f),
            FVector2D(0.f, 1.f),
            AvgSliceSize
        );

    float SliceCountFactor =
        FMath::GetMappedRangeValueClamped(
            FVector2D( 3.f, 5.f ), FVector2D( 0.f, 1.f ), QTESlices.Num()
        );

    float SpeedFactor =
        FMath::GetMappedRangeValueClamped(
            FVector2D( 0.7f, 2.f ), FVector2D( 0.f, 1.f ), SpeedMult
        );

    return FMath::Clamp(
        SliceWidthFactor * 0.35f +
        SliceCountFactor * 0.30f +
        SpeedFactor * 0.35f,
        0.f, 1.f
    );
}

int32 UFishingQTEWidget::ComputeFinalScore() const
{
    float Difficulty = ComputeDifficulty();

    float ExecutionQuality =
        (SuccessfulSlices > 0)
        ? AccumulatedAccuracy / SuccessfulSlices
        : 0.f;

    float DifficultyScoreToType = FMath::FloorToInt(Difficulty * 10) * 10000;
    DifficultyScoreToType = FMath::Clamp(DifficultyScoreToType, 0, 9);

    float ExecutionQualityScore =  FMath::RoundToInt(
        ExecutionQuality * 9999.f
    );

    return FMath::RoundToInt(DifficultyScoreToType * 10000 + ExecutionQualityScore);
}
