
#include "CircleTarget.h"
#include "../AI/Enemy.h"
#include "../AI/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UCircleTarget::UCircleTarget()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;              
}

EBTNodeResult::Type UCircleTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	Enemy->StartCircling();

	TWeakObjectPtr<UBehaviorTreeComponent> WeakBT(&OwnerComp);
	GetWorld()->GetTimerManager().SetTimer(CircleTimerHandle, [this, WeakBT]()
	{
		if (!WeakBT.IsValid()) return;
		UBehaviorTreeComponent* BTComp = WeakBT.Get();
		if (BTComp->GetAIOwner())
		{
			if (AEnemy* E = Cast<AEnemy>(BTComp->GetAIOwner()->GetPawn()))
				E->StopCircling();
		}
		if (UBlackboardComponent* BB = BTComp->GetBlackboardComponent())
		{
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Engaged);
		}
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}, CirclePlayerDuration, false);

	return EBTNodeResult::InProgress;
}

void UCircleTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
		Enemy->EnemyCirclePlayer(DeltaSeconds);
}