
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/Sword.h"
#include "Animation/AnimMontage.h"
#include "Interfaces/Damageable.h"
#include "WeaponComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UWeaponComponent();

protected:

	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TSubclassOf<ASword> SwordClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName StashSocket = "WeaponStash";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FName HandSocket = "hand_weapon";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ASword* SwordInstance = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<UAnimMontage*> AnimMontages;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	int32 AttackIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	bool bHasWeaponInHand = false;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeDamage = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceDistance;
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TraceSocket;
	
	UFUNCTION(BlueprintCallable, Category = "Components")
	void WeaponToHand();
	UFUNCTION(BlueprintCallable, Category = "Components")
	void WeaponToStash();
	UFUNCTION(BlueprintCallable, Category = "Components")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Components")
	void DoAttackTrace(FName TraceSocketName, float TraceRadius);
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetCanAttack(bool bCanAttack);
	UFUNCTION(BlueprintNativeEvent, Category = "Components")
	void PlayVFX();

private:
	
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;
	UPROPERTY()
	USkeletalMeshComponent* OwnerMesh = nullptr;
	UPROPERTY()
	bool CanAttack = true;

};
