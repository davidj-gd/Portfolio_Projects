#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class ATwoPOneCCharacter;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAbilityComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Ability")
	void UseAbility();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable ,Category = "Ability")
	void AbilityEffect();

private:
	
	UPROPERTY()
	ATwoPOneCCharacter* OwnerCharacter = nullptr;
};
