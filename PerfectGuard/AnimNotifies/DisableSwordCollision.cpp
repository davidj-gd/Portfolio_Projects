#include "DisableSwordCollision.h"
#include "../Components/CombatComponent.h"
#include "../Player/MainCharacter.h"

void UDisableSwordCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSequence,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, AnimSequence, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if(!MainCharacter) return;
	UCombatComponent* CombatComp = MainCharacter->GetCombatComponent();
	if (!CombatComp) return;
	MainCharacter->GetCombatComponent()->DisableCollision();
}
