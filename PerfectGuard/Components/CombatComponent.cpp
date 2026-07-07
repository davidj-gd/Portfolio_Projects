#include "CombatComponent.h"
#include "../Player/MainCharacter.h"
#include "CS/Interfaces/Damageable.h"
#include "Kismet/GameplayStatics.h"
#include "../AI/Enemy.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<AMainCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->LeftSwordCollider->OnComponentBeginOverlap.AddDynamic(this, &UCombatComponent::OnHitboxOverlap);
		OwnerCharacter->RightSwordCollider->OnComponentBeginOverlap.AddDynamic(this, &UCombatComponent::OnHitboxOverlap);
	}
	
	ActorHitThisSwing.Empty();
}

void UCombatComponent::EnableCollision()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->LeftSwordCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OwnerCharacter->RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordSounds, OwnerCharacter->GetActorLocation());
	OwnerCharacter->SwordTrailStart();
}

void UCombatComponent::DisableCollision()
{
	if (!OwnerCharacter) return;
	OwnerCharacter->LeftSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OwnerCharacter->RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ActorHitThisSwing.Empty();
	OwnerCharacter->SwordTrailEnd();
}

void UCombatComponent::OnHitboxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;
	if (OtherActor)
	{
		if (ActorHitThisSwing.Contains(OtherActor)) return;
		ActorHitThisSwing.Add(OtherActor);
		IDamageable* Damageable = Cast<IDamageable>(OtherActor);
		if (!Damageable) return;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlashHitSounds, OwnerCharacter->GetActorLocation());
		OwnerCharacter->ApplyHitStop(0.07f);
		OwnerCharacter->DoCameraShake();
		if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
		{
			if (Enemy->bIsGuarding && OwnerCharacter)
			{
				OwnerCharacter->SpawnParryEffect();   
			}
		}
		
		Damageable->ReceiveDamage(DamageAmount);     
		
		if (BloodSplatter.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, BloodSplatter.Num() - 1);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatter[Index], OtherComp->GetComponentLocation());
		}
	}
}

void UCombatComponent::DoAttack()
{
	if (!OwnerCharacter) return;
	if (bComboWindowOpen)
	{
		bComboBufferAttack = true;
		return;
	}
	else if (OwnerCharacter->bIsRunning)
	{
		AttackIndex = 0;
		OwnerCharacter->PlayAnimMontage(RunAttackMontage);
	}
	else
	{
		bComboBufferAttack = false;
		AttackIndex = 0;
		OwnerCharacter->PlayAnimMontage(AnimationMontage[AttackIndex]);
	}
}

void UCombatComponent::DoCombo()
{
	bComboBufferAttack = false;
	AttackIndex++;
	if (AttackIndex >= AnimationMontage.Num()) AttackIndex = 0;
	OwnerCharacter->PlayAnimMontage(AnimationMontage[AttackIndex]);
}

void UCombatComponent::DoParry()
{
	OwnerCharacter->PlayAnimMontage(ParryAnimMontage);
}

void UCombatComponent::DoStopParry()
{
	bIsParrying = false;
	bPerfectParry = false;
	OwnerCharacter->PlayAnimMontage(StopParryAnimMontage);
}

void UCombatComponent::DoSuperAttack()
{
	if (!OwnerCharacter) return;
	bIsSuperAttacking = true;
	OwnerCharacter->PlayAnimMontage(SuperAttackMontage);
}
