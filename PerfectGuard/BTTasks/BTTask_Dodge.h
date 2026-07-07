
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Dodge.generated.h"


UCLASS()
class CS_API UBTTask_Dodge : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_Dodge();
	
public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	FTimerHandle DodgeTimerHandle;
	UPROPERTY(EditAnywhere)
	float DodgeDuration = 0.6f;
};
