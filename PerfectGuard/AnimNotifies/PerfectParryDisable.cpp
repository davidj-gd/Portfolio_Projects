
#include "PerfectParryDisable.h"

#include "../Components/CombatComponent.h"
#include "../Player/MainCharacter.h"

void UPerfectParryDisable::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (!MainCharacter) return;
	UCombatComponent* CombatComp = MainCharacter->GetCombatComponent();
	if (!CombatComp) return;
	CombatComp->bPerfectParry = false;
}
