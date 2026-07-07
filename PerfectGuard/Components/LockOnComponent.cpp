
#include "LockOnComponent.h"

#include "../Player/MainCharacter.h"
#include "../AI/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"

ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AMainCharacter>(GetOwner());
}

void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsLockedOn) return;

	// Target dead / gone / out of range > try to grab another, if none, unlock
	if (!IsValidTarget(CurrentTarget))
	{
		FindTarget();
		if (!IsValidTarget(CurrentTarget))
		{
			ExitLockOn();
			return;
		}
	}

	FVector Direction = (CurrentTarget->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FRotator LookAt = Direction.Rotation();
	// Smoothly swing the camera toward the target
	FRotator SmoothControl = FMath::RInterpTo(OwnerCharacter->GetControlRotation(), LookAt, DeltaTime, CameraInterpSpeed);
	OwnerCharacter->GetController()->SetControlRotation(SmoothControl);

	Direction.Z = 0.f;
	FRotator FaceTarget = Direction.Rotation();
	OwnerCharacter->SetActorRotation(FMath::RInterpTo(OwnerCharacter->GetActorRotation(), FaceTarget, DeltaTime, 20.f));
}

bool ULockOnComponent::IsValidTarget(AActor* Target) const
{
	if (!Target || !OwnerCharacter) return false;
	AEnemy* Enemy = Cast<AEnemy>(Target);
	if (!Enemy || Enemy->GetHealth() <= 0.f) return false;
	return FVector::Dist(OwnerCharacter->GetActorLocation(), Target->GetActorLocation()) <= LockOnRange;
}

void ULockOnComponent::FindTarget()
{
	if (!OwnerCharacter) return;
	
	TArray<AActor*> FoundActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		OwnerCharacter->GetActorLocation(),
		LockOnRange,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		AEnemy::StaticClass(),
		TArray<AActor*>(),
		FoundActors);
	
	float ClosestDistance = MAX_FLT;
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : FoundActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (!Enemy || Enemy->GetHealth() <= 0.f) continue;   // skip dead enemies
		float Distance = FVector::Dist(OwnerCharacter->GetActorLocation(), Actor->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	SetTarget(ClosestActor);
}

void ULockOnComponent::SwitchTarget(float Direction)
{
	if (!bIsLockedOn || !CurrentTarget || !OwnerCharacter) return;

	TArray<AActor*> FoundActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
		OwnerCharacter->GetActorLocation(),
		LockOnRange,
		TArray<TEnumAsByte<EObjectTypeQuery>>(),
		AEnemy::StaticClass(),
		TArray<AActor*>(),
		FoundActors);

	// Cameras right vector, which way is "right" on screen
	const FVector CamRight = FRotationMatrix(OwnerCharacter->GetControlRotation()).GetUnitAxis(EAxis::Y);
	const FVector PlayerLoc = OwnerCharacter->GetActorLocation();

	AActor* Best = nullptr;
	float BestDistance = MAX_FLT;
	for (AActor* Actor : FoundActors)
	{
		if (Actor == CurrentTarget) continue; // Continue if this actor is currently our actor or is invalid
		if (!IsValidTarget(Actor)) continue;

		const FVector ToActor = Actor->GetActorLocation() - PlayerLoc;
		const float Side = FVector::DotProduct(ToActor, CamRight);  
		// Only consider enemies on the flicked side
		if (Direction > 0.f && Side <= 0.f) continue;
		if (Direction < 0.f && Side >= 0.f) continue;

		const float Distance = FVector::Dist(PlayerLoc, Actor->GetActorLocation());
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			Best = Actor;
		}
	}
	if (Best)
	{
		SetTarget(Best);
	}
}

void ULockOnComponent::EnterLockOn()
{
	FindTarget();
	if (CurrentTarget != nullptr)
	{
		bIsLockedOn = true;
		if (OwnerCharacter)
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void ULockOnComponent::ExitLockOn()
{
	bIsLockedOn = false;
	SetTarget(nullptr);
	if (OwnerCharacter)
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ULockOnComponent::SetTarget(AActor* NewTarget)
{
	if (AEnemy* Old = Cast<AEnemy>(CurrentTarget))
	{
		Old->SetLockOn(false);
	}
	
	CurrentTarget = NewTarget;
	
	if (AEnemy* New = Cast<AEnemy>(CurrentTarget))
	{
		New->SetLockOn(true);
	}
}

