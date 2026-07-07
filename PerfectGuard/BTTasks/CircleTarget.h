#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CircleTarget.generated.h"


UCLASS()
class CS_API UCircleTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	UCircleTarget();
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float CirclePlayerDuration = 3.f;
	FTimerHandle CircleTimerHandle;
};
