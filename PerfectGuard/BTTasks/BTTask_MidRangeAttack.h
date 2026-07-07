
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MidRangeAttack.generated.h"


UCLASS()
class CS_API UBTTask_MidRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTask_MidRangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	float AttackDuration = 3.f;
	FTimerHandle AttackTimerHandle;
};
