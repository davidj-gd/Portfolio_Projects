// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interacting.h"
#include "InputActionValue.h"
#include "InteractComponent.generated.h"

UCLASS()
class HAZELIGHT_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()
	
	IInteracting* CurrentInteractable = nullptr;

public:
	void SetCurrentInteractable(IInteracting* NewInteractable);
	void ClearCurrentInteractable(IInteracting* Interactable);
	void BeginInteraction();
	void EndInteraction();
	void HandleInput(const FInputActionValue& Value);

	bool bIsInteracting = false;
	
};
