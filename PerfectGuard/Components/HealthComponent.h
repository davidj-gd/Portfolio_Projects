#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, float, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHealthComponent();
	
	virtual void BeginPlay() override;
	
	void TakeDamage(float Amount);
	bool IsDead() const;
	
	// Healthbar helper
	UFUNCTION(BlueprintPure, Category = Health)
	float GetHealthPercent() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamaged;
	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;
	
	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth = 100.f;
	UPROPERTY(BlueprintReadOnly, Category = Health)
	float CurrentHealth;
};
