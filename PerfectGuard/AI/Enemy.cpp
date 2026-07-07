
#include "Enemy.h"

#include "EnemyAIController.h"
#include "Animation/AnimMontage.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "../Components/CombatComponent.h"
#include "Components/WidgetComponent.h"
#include "CS/Components/HealthComponent.h"
#include "CS/Player/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	WeaponHitbox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WeaponCollider"));
	WeaponHitbox->SetupAttachment(GetMesh(), FName("hand_r"));
	
	LockOnIndicator = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnIndicator"));
	LockOnIndicator->SetupAttachment(GetMesh());
	LockOnIndicator->SetRelativeLocation(FVector(0, 0, 120));
	LockOnIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnIndicator->SetDrawSize(FVector2D(48, 48));
	LockOnIndicator->SetVisibility(false);
}

void AEnemy::BeginPlay()
{
	HealthComponent->MaxHealth = Stats.MaxHealth;
	Super::BeginPlay();
	
	HitAnimIndex = 0;
	WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponHitbox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnHitboxOverlap);
	DamageAmount = Stats.DamageAmount;
}

#pragma region Health

void AEnemy::ReceiveDamage(float Amount)
{
	if (HealthComponent->IsDead()) return;
	
	ApplyHitStop(0.07f);
	
	HitsSinceDodge++;
	if (HitsSinceDodge >= DodgeThreshold && FMath::RandRange(0, 99) < DodgeChance)
	{
		HitsSinceDodge = 0;
		if (AEnemyAIController* AIC = Cast<AEnemyAIController>(GetController()))
			AIC->TriggerDodge();     
		return;                      
	}
	
	if (bIsGuarding)
	{
		PlayAnimMontage(GuardBreakAnimation);
		bIsGuarding = false;
		float FinalAmount = Amount * 0.5f;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GuardBreakSound, GetActorLocation());
		HealthComponent->TakeDamage(FinalAmount);
		return;
	}
	
	if (APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (UCombatComponent* CombatComp = Player->FindComponentByClass<UCombatComponent>())
		{
			if (CombatComp->AttackIndex >= 3)
			{
				float FinalAmount = Amount * 1.2f;
				PlayAnimMontage(HeavyHitAnimation);
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
				HealthComponent->TakeDamage(FinalAmount);
				return;
			}
		}
	}
	
	if (HitAnimations.Num() == 0) return;
	
	if (HitAnimIndex >= HitAnimations.Num())
	{
		HitAnimIndex = 0;
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
	PlayAnimMontage(HitAnimations[HitAnimIndex]);
	HitAnimIndex++;
	
	HealthComponent->TakeDamage(Amount);
}

void AEnemy::HandleDeath()
{
	ActorHitThisSwing.Empty();
	DisableWeaponCollider();
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->BrainComponent->StopLogic("Dead");
	}
	
	PlayAnimMontage(DeathAnimation);
	FadeHealthbarWidget();
	SetLifeSpan(6.f);
}
#pragma endregion

#pragma region Combat

void AEnemy::OnHitboxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(GetController()))
	{
		if (TeamAgent->GetTeamAttitudeTowards(*OtherActor) != ETeamAttitude::Hostile) return;
	}
	
	if (UCombatComponent* CombatComp = OtherActor->FindComponentByClass<UCombatComponent>())
	{
		if (CombatComp->bIsSuperAttacking)
			return;
	}
	
	if (OtherActor)
	{
		if (ActorHitThisSwing.Contains(OtherActor)) return;
		ActorHitThisSwing.Add(OtherActor);
		IDamageable* Damageable = Cast<IDamageable>(OtherActor);
		if (!Damageable) return;
		Damageable->ReceiveDamage(DamageAmount);
		
		UCombatComponent* CombatComp = OtherActor->FindComponentByClass<UCombatComponent>();
		if (CombatComp && CombatComp->bPerfectParry)
		{
			PlayAnimMontage(HeavyHitAnimation);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
			ApplyHitStop(0.1f);
			DisableWeaponCollider();
			ActorHitThisSwing.Empty();
			return;
		}
		
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlashHitSound, GetActorLocation());
	}
}

void AEnemy::EnemyAttack()
{
	FVector TargetLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetActorRotation(Direction.Rotation());
	if (AttackAnimations.Num() == 0) return;
	int RandomAttack = FMath::RandRange(0, AttackAnimations.Num() - 1);
	PlayAnimMontage(AttackAnimations[RandomAttack]);

}

void AEnemy::EnemyMidRangeAttack()
{
	FVector TargetLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	SetActorRotation(Direction.Rotation());
	if (MidRangeAttackAnimation.Num() == 0) return;
	int RandomAttack = FMath::RandRange(0, MidRangeAttackAnimation.Num() - 1);
	PlayAnimMontage(MidRangeAttackAnimation[RandomAttack]);
}

void AEnemy::EnableWeaponCollider()
{
	WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackGruntSound, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSwooshSound, GetActorLocation());
}

void AEnemy::DisableWeaponCollider()
{
	WeaponHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActorHitThisSwing.Empty();
}

void AEnemy::EnemyGuard()
{
	PlayAnimMontage(GuardAnimation);
	bIsGuarding = true;
}

void AEnemy::EnemyCirclePlayer(float DeltaTime)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return;
	
	FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	ToPlayer.Z = 0.f;
	const float Distance = ToPlayer.Size();
	const FVector DirToPlayer = ToPlayer.GetSafeNormal();
	
	const FVector Tangent = FVector::CrossProduct(DirToPlayer, FVector::UpVector);
	
	const float RadialError = Distance - CircleRadius;
	const FVector Radial = DirToPlayer * FMath::Clamp(RadialError / CircleRadius, -1.f, 1.f);
	
	const FVector MoveDir = (Tangent * StrafeDirection + Radial).GetSafeNormal();
	AddMovementInput(MoveDir, 1.f);
	
	FRotator FaceRot = DirToPlayer.Rotation();
	FaceRot.Pitch = 0.f;
	FaceRot.Roll  = 0.f;
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), FaceRot, DeltaTime, 10.f));
}

void AEnemy::StartCircling()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	StrafeDirection = FMath::RandBool() ? 1.f : -1.f;
	bIsCircling = true;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
}

void AEnemy::StopCircling()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bIsCircling = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AEnemy::EnemyDodge()
{
	if (!DodgeAnimation) return;
	PlayAnimMontage(DodgeAnimation);
}

void AEnemy::CaptureForFinisher(FVector NewLocation, FRotator NewRotation)
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->BrainComponent->StopLogic(TEXT("Finisher"));
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AEnemy::ReleaseFromFinisher()
{

	if (HealthComponent->IsDead()) return;

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->BrainComponent->RestartLogic();
	}
}

#pragma endregion

void AEnemy::SetLockOn(bool bLocked)
{
	LockOnIndicator->SetVisibility(bLocked);
}

void AEnemy::FadeHealthbarWidget_Implementation()
{
}

