
#include "EndSuperAttack.h"
#include "../Player/MainCharacter.h"

void UEndSuperAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if(!MainCharacter) return;
	MainCharacter->EndSuperAttack();
}
