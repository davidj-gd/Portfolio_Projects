#pragma once

#include "CoreMinimal.h"
#include "../CSStates.h"
#include "CS/CombatCharacter/CombatCharacter.h"
#include "MainCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;
class UCombatComponent;
class ULockOnComponent;
class UCapsuleComponent;
struct FInputActionValue;
class UNiagaraSystem;
class AEnemy;


UCLASS()
class CS_API AMainCharacter : public ACombatCharacter
{
	GENERATED_BODY()

public:

	AMainCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void ReceiveDamage(float Amount) override;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Blueprintable)
	UCombatComponent* CombatComponent;
	UPROPERTY(VisibleAnywhere)
	ULockOnComponent* LockOnComponent;
	
	// Input properties
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* AttackAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* DodgeAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* CamLockOnAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* RunAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ParryAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* SwitchLockOnTargetAction;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* SuperAttackAction;
	
	// States
	EPlayerState CurrentState;
	
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float WalkRunInterpSpeed = 4.f;

	// Input functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopMove();
	void Attack();
	void Dodge();
	void LockOnTarget();
	void Run();
	void StopRun();
	void Parry();
	void StopParry();
	void SwitchLockOnTarget(const FInputActionValue& Value);
	void SuperAttack();   

public:

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void OnEnterState(EPlayerState State);
	bool TryTransitionToState(EPlayerState NewState);
	
	UFUNCTION()
	void EndSuperAttack();
	
	// Activated/Disabled in BP
	UFUNCTION(BlueprintNativeEvent)
	void SwordTrailStart();
	UFUNCTION(BlueprintNativeEvent)
	void SwordTrailEnd();

	UFUNCTION(BlueprintCallable)
	UCombatComponent* GetCombatComponent() const;
	
	UFUNCTION(BlueprintCallable)
	void CaptureFinisherTarget();
	UFUNCTION(BlueprintCallable)
	void ReleaseFinisherTarget();
	UFUNCTION(BlueprintNativeEvent)
	void ViewDeathScreen();
	UFUNCTION(BlueprintNativeEvent)
	void SpawnParryEffect();
	
	// Camera shake
	UFUNCTION(BlueprintNativeEvent, Category = Camera)
	void DoCameraShake();
	
	// Super attack config
	UPROPERTY(EditDefaultsOnly, Category = Finisher)
	float FinisherDistance = 150.f;
	UPROPERTY(EditDefaultsOnly, Category = Finisher)
	float FinisherSearchRadius = 800.f;
	UPROPERTY()
	AEnemy* CapturedEnemy;
	
	// Sword Colliders
	UPROPERTY(EditAnywhere, Category = Combat)
	UCapsuleComponent* LeftSwordCollider;
	UPROPERTY(EditAnywhere, Category = Combat)
	UCapsuleComponent* RightSwordCollider;

	UPROPERTY()
	bool bIsRunning = false;
	
private:

	virtual void HandleDeath() override;

	bool CanTransitionToState(EPlayerState NewState) const;
	
	UFUNCTION()
	AActor* FindAttackTarget();
	
	// Montages
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
	UAnimMontage* DodgeAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Hitstun)
	TArray<UAnimMontage*> HitstunAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = Parry)
	UAnimMontage* ParryBreakMontage;
	UPROPERTY(EditDefaultsOnly, Category = Death)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = Hitstun)
	UAnimMontage* HeavyHitstunMontage;
	
	// Dodge directional
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
	UAnimMontage* DodgeForward;
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
	UAnimMontage* DodgeBackward;
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
	UAnimMontage* DodgeLeft;
	UPROPERTY(EditDefaultsOnly, Category = Dodge)
	UAnimMontage* DodgeRight;
	
	// Cache dodge direction input
	UPROPERTY()
	FVector2D LastMoveInput = FVector2D::ZeroVector;
	
	UPROPERTY()
	int32 HitstunAnimIndex = 0;
	FTimerHandle HitstunRecoverHandle;
	
	// Audio
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* HitstunSound;
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* ParryBreakSound;
	UPROPERTY(EditDefaultsOnly, Category = Audio)
	USoundBase* PerfectParrySound;

	// VFX
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TArray<UNiagaraSystem*> BloodSplatter;
	
	// Used for saving the actor we are currently attacking so we can face their direction
	UPROPERTY()
	AActor* AttackActor;
};
