#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CoreTypes.h"
#include "BTTask_Attack.generated.h"


UCLASS()
class CS_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Attack();
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	float AttackDuration = 2.f;
	FTimerHandle AttackTimerHandle;
};
