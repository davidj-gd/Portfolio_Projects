#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class TWOPONEC_API IDamageable
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ApplyDamage(float Amount, AActor* DamageCauser) = 0;
	
};
