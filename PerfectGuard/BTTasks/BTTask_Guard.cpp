
#include "BTTask_Guard.h"
#include "../AI/Enemy.h"
#include "../AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_Guard::UBTTask_Guard()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	Enemy->EnemyGuard();
	TWeakObjectPtr<UBehaviorTreeComponent> WeakBT(&OwnerComp);
	GetWorld()->GetTimerManager().SetTimer(GuardTimerHandle, [this, WeakBT]()
	{
		if (!WeakBT.IsValid()) return;
		UBehaviorTreeComponent* BTComp = WeakBT.Get();
		if (UBlackboardComponent* BB = BTComp->GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Engaged);
		}
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}, GuardDuration, false);

	return EBTNodeResult::InProgress;
}
