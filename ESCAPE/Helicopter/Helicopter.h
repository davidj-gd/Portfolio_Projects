
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Helicopter.generated.h"

class APistol;

UCLASS()
class HAZELIGHT_API AHelicopter : public AActor
{
	GENERATED_BODY()

public:

	AHelicopter();

protected:

	virtual void BeginPlay() override;
	virtual float TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator,
	AActor* DamageCauser
	) override;
	UFUNCTION(BlueprintNativeEvent)
	void HelicopterDie();
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealth;

public:

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float CurrentHealth;
};
