#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, InCurrent, float, InMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDeath);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHealthComponent();

protected:

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void HandleDeath();

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthDeath OnHealthDeath;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ApplyDamage(float Amount, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsAlive() const;
};
