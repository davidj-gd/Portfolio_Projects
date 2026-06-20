// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Interactable/InteractComponent.h"
#include "ChDark.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMapping;
struct FInputActionValue;

UCLASS()
class HAZELIGHT_API AChDark : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChDark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MouseLookAction;

	/** Applied to mouse deltas only (gamepad Look uses 1:1). Tweak in BP if view spins too fast. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Look", meta = (ClampMin = "0.01", ClampMax = "2.0"))
	FVector2D MouseLookSensitivity = FVector2D(0.25f, 0.25f);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;
	
	// Camera Components
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Interact")
	UInteractComponent* InteractComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsHanging;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	bool bIsPressingButton;
	
	bool bIsInteracting;

	bool bOrientRotationToMovementBeforeInteract = false;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void Run();
	void StopRun();
	UFUNCTION(BlueprintCallable)
	void OnInteractPressed();
	
	void DoMove(float Right, float Forward);
	void DoLook(float Yaw, float Pitch);
};
