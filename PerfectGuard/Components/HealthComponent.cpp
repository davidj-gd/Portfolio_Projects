
#include "HealthComponent.h"


UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UHealthComponent::TakeDamage(float Amount)
{
	if (IsDead()) return;
	CurrentHealth = FMath::Max(0.f, CurrentHealth - Amount);
	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}
	else
	{
		OnDamaged.Broadcast(Amount);
	}
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0;
}

float UHealthComponent::GetHealthPercent() const
{
    return MaxHealth > 0.f ? CurrentHealth / MaxHealth : 0.f;
}