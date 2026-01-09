// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "FishingGameInstance.h"

#include "FishingCharacter.generated.h"

class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UNiagaraComponent;

struct FInputActionValue;
struct FFishType;

//TODO : delegate for start of QTE and end of QTE
//TODO : Point system for success

UCLASS()
class FISHITUP_API AFishingCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFishingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* QTEIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QTE_DirectionAction;

	// Movement tuning
	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float MaxSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float Acceleration = 800.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float Deceleration = 600.f;

	UPROPERTY(EditAnywhere, Category = "Boat Movement")
	float TurnRate = 15.f; // degrees per second at full speed

	// Runtime
	float CurrentSpeed = 0.f;
	float SteeringInput = 0.f;
	float ThrottleInput = 0.f;

	UPROPERTY(EditDefaultsOnly)
	UMaterialParameterCollection* WindMPC;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> FishingAlertWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* FishingAlertWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> FishingQTEWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* FishingQTEWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> FishingResultClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* FishingResultWidget;

	UPROPERTY()
	bool bFishingAlertActive = false;

	UPROPERTY()
	bool bQTEActive = false;

private:
	UFishingGameInstance* GI;

	UFishSaveGame* SaveGame;

	void UpdateWind();

protected:

	void ShowFisingAlert();
	void HideFishingAlert();

	void ShowFishingQTE();
	void HideFishingQTE();

	UFUNCTION()
	void HandleQTESuccess(int32 score);
	UFUNCTION()
	void HandleQTEFail();

	void OnInteractPressed();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void QTE_Direction(const FInputActionValue& Value);

	void UpdateBoatMovement(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void FishingResultUI(
		const FString& FishName,
		UTexture2D* FishImage,
		bool bDidWin
	);

public:


};
