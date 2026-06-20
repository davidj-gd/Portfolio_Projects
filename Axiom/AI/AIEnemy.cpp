#include "AIEnemy.h"

AAIEnemy::AAIEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent = FindComponentByClass<UHealthComponent>();
}

void AAIEnemy::ApplyDamage(float Amount, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(Amount, DamageCauser);
	}
}

void AAIEnemy::DoAttackTrace(float TraceRadius)
{
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + GetActorForwardVector() * TraceDistance;

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(WeaponMeleeTrace), false, this);
	CollisionParams.AddIgnoredActor(this);

	TArray<FHitResult> Hits;
	const bool bHit = GetWorld()->SweepMultiByObjectType(
		Hits,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ObjectParams,
		Sphere,
		CollisionParams);

	if (!bHit) return;

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActor == this) continue;

		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			Damageable->ApplyDamage(MeleeDamage, this);
			return;
		}
	}
}

void AAIEnemy::AttackTarget_Implementation()
{
}

