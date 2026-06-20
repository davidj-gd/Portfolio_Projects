#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddKey(FName KeyID)
{
	if (KeyID.IsNone()) return;
	if (!CollectedKeys.Contains(KeyID))
	{
		CollectedKeys.Add(KeyID);
	}
}

bool UInventoryComponent::HasKey(FName KeyID) const
{
	return CollectedKeys.Contains(KeyID);
}

void UInventoryComponent::UseKey(FName KeyID)
{
	CollectedKeys.Remove(KeyID);
}


