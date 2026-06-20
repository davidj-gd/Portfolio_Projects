// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interacting.h"
#include "CraneSwing.generated.h"

class USceneComponent;
class ACharacter;

UCLASS()
class HAZELIGHT_API ACraneSwing : public AActor, public IInteracting
{
	GENERATED_BODY()

public:
	ACraneSwing();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteractStart(AActor* InteractingActor) override;
	virtual void OnReceiveInput(const FInputActionValue& Value) override;
	virtual void OnInteractEnd(AActor* InteractingActor) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Crane Swing")
	USceneComponent* CraneRoot;
	UPROPERTY(VisibleAnywhere, Category = "Crane Swing")
	USceneComponent* InteractionSeat;
	UPROPERTY()
	AActor* CurrentUser = nullptr;
	
	UFUNCTION(BlueprintCallable, Category = "Crane Swing")
	void TriggerCraneSwing(AActor* InteractingActor);

	bool bHasCachedCharacterRotationSettings = false;
	bool bCachedUseControllerRotationYaw = true;
	bool bCachedOrientRotationToMovement = true;
};
