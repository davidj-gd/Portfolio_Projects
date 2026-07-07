#pragma once

#include "CoreMinimal.h"
#include "CSStates.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Moving,
	Attacking,
	Dodging,
	Hitstun,
	Dead,
	Parry
};