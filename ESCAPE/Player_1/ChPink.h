
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Interactable/InteractComponent.h"
#include "Pistol/Pistol.h"
#include "Chase/BikeChaseComponent.h"
#include "ChPink.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMapping;
class USceneComponent;
struct FInputActionValue;

UCLASS()
class HAZELIGHT_API AChPink : public ACharacter
{
	GENERATED_BODY()

public:

	AChPink();

protected:

	virtual void BeginPlay() override;
	
	// Inputs
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* BikeInputContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Look", meta = (ClampMin = "0.01", ClampMax = "2.0"))
	FVector2D MouseLookSensitivity = FVector2D(0.25f, 0.25f);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CraneAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* AimAction;
	
	// Camera Components
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;
	

	bool bFoundPistol = false;
	
	// Bike Component reference
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UBikeChaseComponent* BikeChaseComponent;
	
	// Pistol
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<APistol> PistolClass;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Combat")
	APistol* PistolInstance;

public:


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Interact")
	UInteractComponent* InteractComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	TObjectPtr<AActor> ChaseBike;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase")
	UCameraComponent* ChaseCamera;

	
	// Bool's
	bool bIsInteracting;
	bool bOrientRotationToMovementBeforeInteract = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAiming;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MouseLook(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void Run();
	void StopRun();
	void OnInteractPressed();
	void OnCraneInput(const FInputActionValue& Value);
	void OnAimPressed();
	void OnAimReleased();
	void OnShoot();
	void DoMove(float Right, float Forward);
	void DoLook(float Yaw, float Pitch);
	void CheckForPistol();
	
	UFUNCTION()
	void ChangeInputMapping();
	UFUNCTION(BlueprintCallable, Category = "Chase")
	void AttachToBike(USceneComponent* SeatComponent);
	UFUNCTION(BlueprintNativeEvent)
	void StartAim();
	UFUNCTION(BlueprintNativeEvent)
	void StopAim();
	UFUNCTION(BlueprintNativeEvent)
	void AttachPistolToHand();
	
};
