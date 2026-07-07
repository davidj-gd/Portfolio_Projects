
#include "BTTask_Dodge.h"
#include "../AI/Enemy.h"
#include "../AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_Dodge::UBTTask_Dodge()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_Dodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;
	Enemy->EnemyDodge();
	TWeakObjectPtr<UBehaviorTreeComponent> WeakBT(&OwnerComp);
	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, [this, WeakBT]()
	{
		if (!WeakBT.IsValid()) return;
		UBehaviorTreeComponent* BTComp = WeakBT.Get();
		if (UBlackboardComponent* BB = BTComp->GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Engaged);
		}
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}, DodgeDuration, false);

	return EBTNodeResult::InProgress;
}
