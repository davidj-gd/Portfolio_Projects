#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraLockOnComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UCameraLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCameraLockOnComponent();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	AActor* CurrentTarget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	float LockOnRange = 1000.f;
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	float UnlockRange = 1500.f;
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	float SwitchTargetMargin = 150.f;
	
	UFUNCTION(BlueprintCallable, Category = "Camera Lock-On")
	AActor* FindClosestValidEnemy();
	UFUNCTION(BlueprintCallable, Category = "Camera Lock-On")
	void UpdateAutoLockOn(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Camera Lock-On")
	bool IsValidLockTarget(AActor* LockTarget);
	UFUNCTION(BlueprintCallable, Category = "Camera Lock-On")
	void UpdateLockOnCamera(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Camera Lock-On")
	bool IsLockedOn() const { return CurrentTarget != nullptr; }
	
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	float CameraInterpSpeed = 6.f;
	UPROPERTY(EditAnywhere, Category = "Camera Lock-On")
	FName TargetAimSocket = "spine_03";
	
	
protected:
	
	UPROPERTY()
	ACharacter* OwnerCharacter;
	UPROPERTY()
	USkeletalMeshComponent* Mesh;

	void UpdateLockOnRotationMode(bool bLocked);
	void RefreshEnemyCache();

	UPROPERTY(EditAnywhere, Category = "Camera Lock-On", meta=(ClampMin=0.05, ClampMax=1.0, Units="s"))
	float EnemyScanInterval = 0.2f;

	TArray<AActor*> CachedEnemies;
	FTimerHandle EnemyScanTimer;
	bool bWasLocked = false;
};
