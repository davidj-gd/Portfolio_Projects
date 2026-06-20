// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractComponent.h"
#include "GameFramework/Actor.h"

void UInteractComponent::SetCurrentInteractable(IInteracting* NewInteractable)
{
	CurrentInteractable = NewInteractable;
}

void UInteractComponent::ClearCurrentInteractable(IInteracting* Interactable)
{
	if (CurrentInteractable == Interactable)
	{
		CurrentInteractable = nullptr;
	}
}

void UInteractComponent::BeginInteraction()
{
	if (!CurrentInteractable)
	{
		return;
	}
	AActor* Owner = GetOwner();
	if (!Owner || !IsValid(Owner) || Owner->HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}
	CurrentInteractable->OnInteractStart(Owner);
	bIsInteracting = true;
}

void UInteractComponent::EndInteraction()
{
	if (CurrentInteractable)
	{
		AActor* Owner = GetOwner();
		if (Owner && IsValid(Owner) && !Owner->HasAnyFlags(RF_ClassDefaultObject))
		{
			CurrentInteractable->OnInteractEnd(Owner);
		}
	}
	bIsInteracting = false;
}

void UInteractComponent::HandleInput(const FInputActionValue& Value)
{
	if (bIsInteracting && CurrentInteractable)
	{
		CurrentInteractable->OnReceiveInput(Value);
	}
}
