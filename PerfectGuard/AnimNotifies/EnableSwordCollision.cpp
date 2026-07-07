#include "EnableSwordCollision.h"
#include "../Components/CombatComponent.h"
#include "../Player/MainCharacter.h"


void UEnableSwordCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if(!MainCharacter) return;
	UCombatComponent* CombatComp = MainCharacter->GetCombatComponent();
	if (!CombatComp) return;
	MainCharacter->GetCombatComponent()->EnableCollision();
}
