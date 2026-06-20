// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "BikeChaseComponent.generated.h"

class AChPink;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HAZELIGHT_API UBikeChaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UBikeChaseComponent();

protected:

	virtual void BeginPlay() override;
	
	UFUNCTION()
	void EnableShootingControlls();
	
	UPROPERTY()
	AChPink* Player1;
	UPROPERTY()
	AActor* Player2;
	UPROPERTY(EditAnywhere, Category = "Chase", meta = (UseComponentPicker = "true"))
	FComponentReference PassengerSeatRef;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void StartChase();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bChaseSceneStarted;
	
};
