#include "CameraLockOnComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "AI/AIEnemy.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

UCameraLockOnComponent::UCameraLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;
	Mesh = Cast<USkeletalMeshComponent>(OwnerCharacter->GetMesh());
	if (!Mesh) return;

	GetWorld()->GetTimerManager().SetTimer(
		EnemyScanTimer,
		this,
		&UCameraLockOnComponent::RefreshEnemyCache,
		EnemyScanInterval,
		true);
}

void UCameraLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateAutoLockOn(DeltaTime);
}

void UCameraLockOnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(EnemyScanTimer);
}

void UCameraLockOnComponent::RefreshEnemyCache()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemy::StaticClass(), CachedEnemies);
}

AActor* UCameraLockOnComponent::FindClosestValidEnemy()
{
	if (!OwnerCharacter) return nullptr;

	AActor* ClosestEnemy = nullptr;
	float ClosestDistanceSq = TNumericLimits<float>::Max();

	const FVector PlayerLocation = OwnerCharacter->GetActorLocation();

	for (AActor* EnemyActor : CachedEnemies)
	{
		if (!IsValidLockTarget(EnemyActor)) continue;

		const float DistSq = FVector::DistSquared(PlayerLocation, EnemyActor->GetActorLocation());
		if (DistSq < ClosestDistanceSq)
		{
			ClosestDistanceSq = DistSq;
			ClosestEnemy = EnemyActor;
		}
	}

	return ClosestEnemy;
}

void UCameraLockOnComponent::UpdateAutoLockOn(float DeltaTime)
{
	if (CurrentTarget && !IsValidLockTarget(CurrentTarget))
	{
		CurrentTarget = nullptr;
	}
	
	if (!CurrentTarget)
	{
		 CurrentTarget = FindClosestValidEnemy();
	}
	
	const bool bIsLocked = CurrentTarget != nullptr;
	if (bIsLocked != bWasLocked)
	{
		UpdateLockOnRotationMode(bIsLocked);
		bWasLocked = bIsLocked;
	}

	if (CurrentTarget)
	{
		UpdateLockOnCamera(DeltaTime);
	}
}

void UCameraLockOnComponent::UpdateLockOnRotationMode(bool bLocked)
{
	if (!OwnerCharacter) return;

	UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement();
	if (!Movement) return;

	if (bLocked)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		Movement->bOrientRotationToMovement = false;
	}
	else
	{
		OwnerCharacter->bUseControllerRotationYaw = false;
		Movement->bOrientRotationToMovement = true;
	}
}

bool UCameraLockOnComponent::IsValidLockTarget(AActor* LockTarget)
{
	if (!LockTarget || !OwnerCharacter) return false;
	if (LockTarget == GetOwner()) return false;
	
	AAIEnemy* Enemy = Cast<AAIEnemy>(LockTarget);
	if (!Enemy) return false;

	if (const UHealthComponent* Health = Enemy->FindComponentByClass<UHealthComponent>())
	{
		if (!Health->IsAlive()) return false;
	}
	
	const float Distance = FVector::Dist(OwnerCharacter->GetActorLocation(), Enemy->GetActorLocation());
	const float MaxRange = (LockTarget == CurrentTarget) ? UnlockRange : LockOnRange;
	
	return Distance <= MaxRange;
}

void UCameraLockOnComponent::UpdateLockOnCamera(float DeltaTime)
{
	if (!OwnerCharacter || !CurrentTarget) return;
	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;

	FVector AimLocation = CurrentTarget->GetActorLocation();
	if (const ACharacter* EnemyChar = Cast<ACharacter>(CurrentTarget))
	{
		if (const USkeletalMeshComponent* EnemyMesh = EnemyChar->GetMesh())
		{
			if (EnemyMesh->DoesSocketExist(TargetAimSocket))
			{
				AimLocation = EnemyMesh->GetSocketLocation(TargetAimSocket);
			}
		}
	}

	const FVector ViewLocation = OwnerCharacter->GetPawnViewLocation();
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ViewLocation, AimLocation);
	const FRotator CurrentRotation = Controller->GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, CameraInterpSpeed);

	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -40.f, 40.f);
	Controller->SetControlRotation(NewRotation);
}