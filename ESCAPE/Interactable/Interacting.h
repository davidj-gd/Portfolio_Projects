// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputActionValue.h"
#include "Interacting.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteracting : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HAZELIGHT_API IInteracting
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnInteractStart(AActor* Player) = 0;
	virtual void OnInteractEnd(AActor* Player) = 0;
	virtual void OnReceiveInput(const FInputActionValue& Value) = 0;

};
