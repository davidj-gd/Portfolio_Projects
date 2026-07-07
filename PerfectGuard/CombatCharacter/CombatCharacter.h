#pragma once

#include "CoreMinimal.h"
#include "../Interfaces/Damageable.h"
#include "../Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

UCLASS(Abstract)
class CS_API ACombatCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:

	ACombatCharacter();

protected:

	virtual void BeginPlay() override;
	
	FTimerHandle HitStopHandle;
	bool bIsHitStopped = false;
	
public:
	
	UFUNCTION()
	virtual void HandleDamage(float Amount);
	UFUNCTION()
	virtual void HandleDeath();
	virtual void ReceiveDamage(float Amount) override {}
	virtual float GetHealth() const override { return HealthComponent->CurrentHealth; }
	
	void ApplyHitStop(float Duration);
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHealthComponent* HealthComponent;
};
