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

void UHealthComponent::ApplyDamage(float Amount, AActor* Instigator)
{
	if (!IsAlive()) return;
	CurrentHealth = FMath::Max(0.f, CurrentHealth - Amount);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	if (!IsAlive())
	{
		HandleDeath();
	}
}

bool UHealthComponent::IsAlive() const
{
	return CurrentHealth > 0.f;
}

void UHealthComponent::HandleDeath()
{
	OnHealthDeath.Broadcast();
}

