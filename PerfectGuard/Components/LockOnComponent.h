
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class AMainCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CS_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	ULockOnComponent();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	AActor* CurrentTarget;
	
	// Lock on settings
	UPROPERTY(EditDefaultsOnly, Category = LockOn)
	float LockOnRange = 2000.f;
	UPROPERTY(EditDefaultsOnly, Category = LockOn)
	float CameraInterpSpeed = 8.f;
	
	// Player ref
	UPROPERTY()
	AMainCharacter* OwnerCharacter;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	bool bIsLockedOn = false;
	UPROPERTY()
	bool bCanSwitchTarget = true;
	
	// How much to move the mouse for the switch to trigger
	UPROPERTY(EditDefaultsOnly, Category = LockOn)
	float MouseSwitchThreshold = 5.f;
	
	UFUNCTION()
	void FindTarget();
	UFUNCTION()
	void EnterLockOn();
	UFUNCTION()
	void ExitLockOn();
	UFUNCTION()
	void SetTarget(AActor* NewTarget);
	void SwitchTarget(float Direction);

private:
	
	bool IsValidTarget(AActor* Target) const;
};
