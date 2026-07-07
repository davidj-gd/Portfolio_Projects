#pragma once

#include "CoreMinimal.h"
#include "Engine/HitResult.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AMainCharacter;
class UPrimitiveComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CS_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCombatComponent();
	
	UFUNCTION()
	void DoAttack();
	UFUNCTION()
	void EnableCollision();
	UFUNCTION()
	void DisableCollision();
	UFUNCTION()
	void DoCombo();
	UFUNCTION()
	void DoParry();
	UFUNCTION()
	void DoStopParry();
	UFUNCTION()
	void DoSuperAttack();
	UFUNCTION()
	void OnHitboxOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY()
	int32 AttackIndex = 0;
	
	UPROPERTY(EditAnywhere, Category = Attack)
	float AttackTargetRange = 200.f;
	
	// Bools
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsParrying = false;
	UPROPERTY(VisibleAnywhere)
	bool bPerfectParry = false;
	UPROPERTY()
	bool bComboWindowOpen = false;
	UPROPERTY()
	bool bComboBufferAttack = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsSuperAttacking = false;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float DamageAmount = 20.f;

private:
	
	// Player ref
	UPROPERTY()
	AMainCharacter* OwnerCharacter;
	
	UPROPERTY()
	TSet<AActor*> ActorHitThisSwing;
	
	// Audio
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* SwordSounds;
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* SlashHitSounds;
	
	// VFX
	UPROPERTY(EditDefaultsOnly, Category = VFX)
	TArray<UNiagaraSystem*> BloodSplatter;
	
	// Montages
	UPROPERTY(EditDefaultsOnly, Category = Parry)
	UAnimMontage* ParryAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Parry)
	UAnimMontage* StopParryAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* RunAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	UAnimMontage* SuperAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Attack)
	TArray<UAnimMontage*> AnimationMontage;
	
};
