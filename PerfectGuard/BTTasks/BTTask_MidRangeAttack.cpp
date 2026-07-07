
#include "BTTask_MidRangeAttack.h"
#include "../AI/Enemy.h"
#include "../AI/EnemyAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_MidRangeAttack::UBTTask_MidRangeAttack()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_MidRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	Enemy->EnemyMidRangeAttack();
	TWeakObjectPtr<UBehaviorTreeComponent> WeakBT(&OwnerComp);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this, WeakBT]()
	{
		if (!WeakBT.IsValid()) return;
		UBehaviorTreeComponent* BTComp = WeakBT.Get();
		if (UBlackboardComponent* BB = BTComp->GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Engaged);
		}
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}, AttackDuration, false);

	return EBTNodeResult::InProgress;
}
