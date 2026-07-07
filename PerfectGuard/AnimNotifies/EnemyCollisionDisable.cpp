
#include "EnemyCollisionDisable.h"
#include "../AI/Enemy.h"

void UEnemyCollisionDisable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSequence,
	const FAnimNotifyEventReference& EventReference)
{
	AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;
	Enemy->DisableWeaponCollider();
}
