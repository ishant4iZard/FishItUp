// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "FishingCharacter.generated.h"

class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

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


	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> FishingAlertWidgetClass;

	UPROPERTY()
	UUserWidget* FishingAlertWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> FishingQTEWidgetClass;

	UPROPERTY()
	UUserWidget* FishingQTEWidget;

	UPROPERTY()
	bool bFishingAlertActive = false;

	UPROPERTY()
	bool bQTEActive = false;

	void ShowFisingAlert();
	void HideFishingAlert();

	void ShowFishingQTE();
	void HideFishingQTE();

	UFUNCTION()
	void HandleQTESuccess();
	UFUNCTION()
	void HandleQTEFail();

	void OnInteractPressed();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void QTE_Direction(const FInputActionValue& Value);

public:


};
