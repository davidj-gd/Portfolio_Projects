#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class CS_API IDamageable
{
	GENERATED_BODY()
	
public:
	
	virtual void ReceiveDamage(float Amount) = 0;
	virtual float GetHealth() const = 0;
	
};