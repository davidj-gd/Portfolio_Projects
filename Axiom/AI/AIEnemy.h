#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Damageable.h"
#include "Components/HealthComponent.h"
#include "AIEnemy.generated.h"

UCLASS()
class TWOPONEC_API AAIEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:

	AAIEnemy();

protected:

	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceDistance = 100.f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeDamage = 10.f;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ApplyDamage(float Amount, AActor* DamageCauser) override;
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void AttackTarget();
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DoAttackTrace(float TraceRadius);
	
};
