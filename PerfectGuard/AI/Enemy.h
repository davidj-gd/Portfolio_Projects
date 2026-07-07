#pragma once

#include "CoreMinimal.h"
#include "CS/CombatCharacter/CombatCharacter.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()
	
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;
	// Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount = 20.f;
	// Behavior
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EngageCooldown = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SurroundDistance = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackChance = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GuardChance = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CircleChance = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EngageRange = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MidRange = 350.f;

};

UCLASS()
class CS_API AEnemy : public ACombatCharacter
{
	GENERATED_BODY()

public:

	AEnemy();

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	UPROPERTY()
	int32 HitAnimIndex;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<UAnimMontage*> AttackAnimations;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<UAnimMontage*> MidRangeAttackAnimation;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	float DamageAmount = 20.f; // Seeded from stats
	
	// Audio
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* HitstunSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* AttackGruntSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* AttackSwooshSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* SlashHitSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* GuardBreakSound;
	
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* WeaponHitbox;
	
	// Circling enemy
	UPROPERTY(EditDefaultsOnly, Category = Circle)
	float CircleRadius = 250.f;
	float StrafeDirection = 1.f;
	UPROPERTY()
	bool bIsCircling = false;
	
	// Dodge settings
	UPROPERTY()
	int32 HitsSinceDodge = 0;
	int32 DodgeThreshold = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DodgeChance = 50;
	
	// Montages
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	TArray<UAnimMontage*> HitAnimations;
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	UAnimMontage* HeavyHitAnimation;
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	UAnimMontage* DeathAnimation;
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	UAnimMontage* GuardAnimation;
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	UAnimMontage* GuardBreakAnimation;
	UPROPERTY(EditAnywhere, Category = HitAnimation)
	UAnimMontage* DodgeAnimation;
	
	UPROPERTY()
	TSet<AActor*> ActorHitThisSwing;
	
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* LockOnIndicator;

public:

	// Overrides
	virtual void ReceiveDamage(float Amount) override;
	virtual void HandleDeath() override;
	
	UFUNCTION()
	void EnemyAttack();
	UFUNCTION()
	void EnemyMidRangeAttack();
	UFUNCTION()
	void EnableWeaponCollider();
	UFUNCTION()
	void DisableWeaponCollider();
	UFUNCTION()
	void EnemyGuard();
	UFUNCTION()
	void EnemyCirclePlayer(float DeltaTime);
	UFUNCTION()
	void SetLockOn(bool bLocked);
	UFUNCTION()
	void EnemyDodge();
	void StartCircling();
	void StopCircling();
	
	UFUNCTION(BlueprintNativeEvent)
	void FadeHealthbarWidget();

	// Finisher capture, snap into place + freeze so a scripted combo always connects
	UFUNCTION(BlueprintCallable)
	void CaptureForFinisher(FVector NewLocation, FRotator NewRotation);
	UFUNCTION(BlueprintCallable)
	void ReleaseFromFinisher();
	
	UPROPERTY()
	bool bIsGuarding = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	FEnemyStats Stats;
	
};
