#include "InteractComponent.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractComponent::Interact(AActor* Interactor)
{
	if (!bIsEnabled || !Interactor) return;
	OnInteract.Broadcast(Interactor);
}