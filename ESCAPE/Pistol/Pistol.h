#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Pistol.generated.h"

UCLASS()
class HAZELIGHT_API APistol : public AActor
{
	GENERATED_BODY()

public:
	APistol();

protected:

	virtual void BeginPlay() override;
	void ResetCooldown();
	void PlaySound();
	void PlayMuzzleFlash();
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* SKPistolMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	USceneComponent* Root;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<USoundBase*> HitSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UNiagaraSystem* ImpactFX;
	
	FTimerHandle ShootingCooldownHandle;
	
	bool bIsOnCooldown = false;

public:

	virtual void Tick(float DeltaTime) override;
	void ShootPistol(const FVector& CamLocation, const FVector& CamForward);
	void FireFromCamera(const FVector& CamLocation, const FVector& CamForward);
	
	// Weapon Stats
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage = 20.0f;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	int MaxAmmo = 20;
	
	// FX
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	USoundBase* BulletSound;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UNiagaraSystem* MuzzleFlashParticle;
};
