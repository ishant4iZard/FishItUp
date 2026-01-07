// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingCharacter.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "FishingQTEWidget.h"

// Sets default values
AFishingCharacter::AFishingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AFishingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    if (FishingAlertWidgetClass)
    {
        FishingAlertWidget = CreateWidget(GetWorld(), FishingAlertWidgetClass);
        if (FishingAlertWidget)
        {
            FishingAlertWidget->AddToViewport();
            FishingAlertWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (FishingQTEWidgetClass)
    {
        FishingQTEWidget = CreateWidget(GetWorld(), FishingQTEWidgetClass);
        FishingQTEWidget->AddToViewport();
        FishingQTEWidget->SetVisibility(ESlateVisibility::Hidden);

        UFishingQTEWidget* temp = Cast<UFishingQTEWidget>(FishingQTEWidget);

        temp->OnQTESuccess.AddDynamic(
            this,
            &AFishingCharacter::HandleQTESuccess
        );

        temp->OnQTEFail.AddDynamic(
            this,
            &AFishingCharacter::HandleQTEFail
        );
    }




    FTimerHandle AlertStartTimer;
    GetWorldTimerManager().SetTimer(
        AlertStartTimer,
        this,
        &AFishingCharacter::ShowFisingAlert,
        10.f,
        true
    );
}

// Called every frame
void AFishingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFishingCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Called to bind functionality to input
void AFishingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AFishingCharacter::OnInteractPressed);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFishingCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFishingCharacter::Look);

        EnhancedInputComponent->BindAction(QTE_DirectionAction, ETriggerEvent::Triggered, this, &AFishingCharacter::QTE_Direction);
    }
}

void AFishingCharacter::ShowFisingAlert()
{
    if (!FishingAlertWidget) return;

    FishingAlertWidget->SetVisibility(ESlateVisibility::Visible);
    
    UE_LOG(LogTemp, Display, TEXT("this started"));

    bFishingAlertActive = true;

    // Auto-hide after 5 seconds
    FTimerHandle RemoveFishingAlertTimer;
    GetWorldTimerManager().SetTimer(
        RemoveFishingAlertTimer,
        this,
        &AFishingCharacter::HideFishingAlert,
        5.f,
        false
    );
}

void AFishingCharacter::HideFishingAlert()
{
    bFishingAlertActive = false;
    if (FishingAlertWidget)
    {
        FishingAlertWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AFishingCharacter::ShowFishingQTE()
{
    UE_LOG(LogTemp, Display, TEXT("FishingQTE started"));

    if (!FishingQTEWidget) return;

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(DefaultMappingContext);
            Subsystem->AddMappingContext(QTEIMC, 1);
        }
    }

    // Switch input state
    bQTEActive = true;
    bFishingAlertActive = false;

    UFishingQTEWidget* QTE = Cast<UFishingQTEWidget>(FishingQTEWidget);


    QTE->BuildSlices(TArray<FQTESlice>(QTE->GenerateQTESlices()));

    FishingQTEWidget->SetVisibility(ESlateVisibility::Visible);

    GetCharacterMovement()->DisableMovement();
}

void AFishingCharacter::HideFishingQTE()
{
    if (!FishingQTEWidget) return;

    bQTEActive = false;
    FishingQTEWidget->SetVisibility(ESlateVisibility::Hidden);

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(QTEIMC);
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AFishingCharacter::OnInteractPressed()
{
    UE_LOG(LogTemp, Display, TEXT("space pressed succesfully"));
    if (!bFishingAlertActive)
        return;

    // Valid fishing window
    HideFishingAlert();
    ShowFishingQTE();
}

void AFishingCharacter::Move(const FInputActionValue& Value)
{
    if (bQTEActive) return;

    UE_LOG(LogTemp, Display, TEXT("I am Moving"));
}

void AFishingCharacter::Look(const FInputActionValue& Value)
{
    if (bQTEActive) return;

    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (!CameraBoom) return;

    // Apply yaw rotation to the spring arm (horizontal look)
    FRotator NewRotation = CameraBoom->GetRelativeRotation();
    NewRotation.Yaw += LookAxisVector.X;
    NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookAxisVector.Y, -80.f, 80.f);
    CameraBoom->SetRelativeRotation(NewRotation);
}

void AFishingCharacter::QTE_Direction(const FInputActionValue& Value)
{
    if (!FishingQTEWidget || !bQTEActive) return;

    const FVector2D Input = Value.Get<FVector2D>();

    // Deadzone
    if (Input.Size() < 0.6f)
        return;

    EQTEDirection Dir;

    if (FMath::Abs(Input.X) > FMath::Abs(Input.Y))
    {
        Dir = (Input.X > 0.f)
            ? EQTEDirection::Right
            : EQTEDirection::Left;
    }
    else
    {
        Dir = (Input.Y > 0.f)
            ? EQTEDirection::Up
            : EQTEDirection::Down;
    }

    (Cast<UFishingQTEWidget>(FishingQTEWidget))->HandleQTEInput(Dir);
}

void AFishingCharacter::HandleQTESuccess()
{
    UE_LOG(LogTemp, Display, TEXT("Fish Caught Succesfully"));
    bQTEActive = false;
    HideFishingQTE();
}

void AFishingCharacter::HandleQTEFail()
{
    UE_LOG(LogTemp, Display, TEXT("uh oh!"));
    FTimerHandle RemoveQTETimer;
    GetWorldTimerManager().SetTimer(
        RemoveQTETimer,
        this,
        &AFishingCharacter::HideFishingQTE,
        1.5f,
        false
    );
}

