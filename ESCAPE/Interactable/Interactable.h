// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Collision.h"
#include "Components/SceneComponent.h"
#include "Interactable.generated.h"

class ACharacter;

UCLASS()
class HAZELIGHT_API AInteractable : public AActor
{
	GENERATED_BODY()

public:

	AInteractable();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

public:

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactable")
	bool bPlayerCanInteract;

	// The actor the player should actually control when overlapping this sphere.
	// This is typically your crane actor that implements `IInteracting`.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interactable")
	AActor* InteractionTarget;
};
