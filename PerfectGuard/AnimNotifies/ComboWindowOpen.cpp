
#include "ComboWindowOpen.h"

#include "../Components/CombatComponent.h"
#include "../Player/MainCharacter.h"

void UComboWindowOpen::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (!MainCharacter) return;
	UCombatComponent* CombatComponent = MainCharacter->GetCombatComponent();
	if (!CombatComponent) return;
	CombatComponent->bComboWindowOpen = true;
}
