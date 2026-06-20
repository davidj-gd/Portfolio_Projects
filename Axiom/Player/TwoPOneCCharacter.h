#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/WeaponComponent.h"
#include "Components/AbilityComponent.h"
#include "Components/CameraLockOnComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/HealthComponent.h"
#include "Interacting/InteractComponent.h"
#include "Weapon/Sword.h"
#include "Interfaces/Damageable.h"
#include "TwoPOneCCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTwoPOneCCharacter, Log, All);

UCLASS(abstract)
class ATwoPOneCCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* RunAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AbilityAction;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* AttackAction;
	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponComponent* WeaponComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UAbilityComponent* AbilityComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraLockOnComponent* CamLockOnComp;
	
	// Extra meshes
	UPROPERTY(EditAnywhere, Category = "Weapon")
	ASword* SwordMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	USkeletalMeshComponent* AbilityMesh;

public:
	
	ATwoPOneCCharacter();	

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float LerpSpeedRate;
	float CurrentSpeedTarget;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ApplyDamage(float Amount, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StartRun();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void StopRun();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void EquipWeapon();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Interact();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnUseAbility();
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnAttack();
	UFUNCTION(BlueprintCallable, Category = "Components")
	void FindInteractable();

	UPROPERTY(EditAnywhere, Category="Components")
	float InteractRange;
	UPROPERTY(EditAnywhere, Category="Components", meta=(ClampMin=0.05, ClampMax=1.0, Units="s"))
	float InteractScanInterval = 0.1f;
	UPROPERTY()
	UInteractComponent* FocusedInteractable = nullptr;

private:
	FTimerHandle InteractScanTimer;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
};

