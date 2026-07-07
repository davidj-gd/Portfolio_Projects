
#include "CombatCharacter.h"
#include "../Components/HealthComponent.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDamaged.AddDynamic(this, &ACombatCharacter::HandleDamage);
	HealthComponent->OnDeath.AddDynamic(this, &ACombatCharacter::HandleDeath);
}

void ACombatCharacter::HandleDamage(float Amount)
{
}

void ACombatCharacter::HandleDeath()
{
}

void ACombatCharacter::ApplyHitStop(float Duration)
{
	if (bIsHitStopped) return;
	bIsHitStopped = true;
	CustomTimeDilation = 0.3f;
	GetWorldTimerManager().SetTimer(HitStopHandle, [this]
	{
		CustomTimeDilation = 1.0f;
		bIsHitStopped = false;
	}, Duration, false);
}

