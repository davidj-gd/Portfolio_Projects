// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interacting.h"
#include "PowerButton/PowerButton.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Crane.generated.h"

class USceneComponent;
class ACharacter;

UCLASS(BlueprintType, Blueprintable)
class HAZELIGHT_API ACrane : public AActor, public IInteracting
{
	GENERATED_BODY()

public:
	ACrane();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteractStart(AActor* Player) override;
	virtual void OnReceiveInput(const FInputActionValue& Value) override;
	virtual void OnInteractEnd(AActor* Player) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
	UAudioComponent* CraneAudioComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane")
	USoundCue* CraneCue;
	UPROPERTY(VisibleAnywhere, Category = "Crane")
	USceneComponent* CraneRoot;
	UPROPERTY(VisibleAnywhere, Category = "Crane")
	USceneComponent* InteractionSeat;
	UPROPERTY()
	AActor* CurrentUser = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Crane Input")
	float RotationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crane|Power")
	TObjectPtr<APowerButton> PowerButtonActor = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Crane|Power")
	bool bIsSteering;
	UPROPERTY(EditAnywhere, Category = "Crane|Power")
	bool bRequireActivatedPowerToSteer = true;

	bool bHasCachedCharacterRotationSettings = false;
	bool bCachedUseControllerRotationYaw = true;
	bool bCachedOrientRotationToMovement = true;

	bool IsCranePowerAllowed() const;
	
	void SetCraneMotionSound(bool bPlaying);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Camera")
	void SetCameraEffect();
	UFUNCTION(BlueprintNativeEvent, Category = "Camera")
	void DisableCameraEffect();
	UFUNCTION(BlueprintNativeEvent, Category = "Widget")
	void ShowWidget();
	UFUNCTION(BlueprintNativeEvent, Category = "Widget")
	 void DisableWidget();
	
};
