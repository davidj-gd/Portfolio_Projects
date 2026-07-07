
#include "EnemyCollisionEnable.h"
#include "../AI/Enemy.h"

void UEnemyCollisionEnable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSequence,
                                   const FAnimNotifyEventReference& EventReference)
{
	AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;
	Enemy->EnableWeaponCollider();
}
