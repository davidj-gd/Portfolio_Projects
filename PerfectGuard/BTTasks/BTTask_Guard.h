#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Guard.generated.h"


UCLASS()
class CS_API UBTTask_Guard : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Guard();
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere)
	float GuardDuration = 2.f;
	FTimerHandle GuardTimerHandle;
};
