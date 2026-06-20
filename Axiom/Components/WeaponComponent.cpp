#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/Sword.h"
#include "Components/SkeletalMeshComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	TraceDistance = 75.f;
	TraceSocket = HandSocket;
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;
	OwnerMesh = Cast<USkeletalMeshComponent>(OwnerCharacter->GetMesh());
	if (!OwnerMesh) return;
	if (!SwordClass) return;
	
	FActorSpawnParameters Params;
	Params.Owner = OwnerCharacter;
	Params.Instigator = OwnerCharacter;
	
	SwordInstance = GetWorld()->SpawnActor<ASword>(SwordClass, FTransform::Identity, Params);
	if (!SwordInstance) return;
	
	SwordInstance->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		StashSocket);
}

void UWeaponComponent::WeaponToHand()
{
	bHasWeaponInHand = true;
	SwordInstance->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			HandSocket);
}

void UWeaponComponent::WeaponToStash()
{
	bHasWeaponInHand = false;
	SwordInstance->AttachToComponent(OwnerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			StashSocket);
}

void UWeaponComponent::Attack()
{
	if (!OwnerCharacter || !OwnerMesh) return;
	if (AnimMontages.Num() == 0) return;
	if (!CanAttack) return;
	
	UAnimMontage* Montage = AnimMontages[AttackIndex % AnimMontages.Num()];
	
	// Increment index and call animation
	AttackIndex ++;
	OwnerCharacter->PlayAnimMontage(Montage);
}

void UWeaponComponent::DoAttackTrace(FName TraceSocketName, float TraceRadius)
{
	if (!OwnerCharacter || !OwnerMesh || !GetWorld()) return;

	const FName SocketName = TraceSocketName.IsNone() ? TraceSocket : TraceSocketName;
	const FVector TraceStart = OwnerMesh->GetSocketLocation(SocketName);
	const FVector TraceEnd = TraceStart + OwnerCharacter->GetActorForwardVector() * TraceDistance;

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);

	FCollisionQueryParams CollisionParams(SCENE_QUERY_STAT(WeaponMeleeTrace), false, OwnerCharacter);
	CollisionParams.AddIgnoredActor(OwnerCharacter);
	if (SwordInstance)
	{
		CollisionParams.AddIgnoredActor(SwordInstance);
	}

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
		if (!HitActor || HitActor == OwnerCharacter) continue;

		if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
		{
			Damageable->ApplyDamage(MeleeDamage, OwnerCharacter);
			PlayVFX();
			return; // Only do damage once per attack
		}
	}
}

void UWeaponComponent::SetCanAttack(bool bCanAttack)
{
	CanAttack = bCanAttack;
}

void UWeaponComponent::PlayVFX_Implementation()
{
}

