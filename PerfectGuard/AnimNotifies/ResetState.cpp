
#include "ResetState.h"
#include "../Player/MainCharacter.h"

void UResetState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
	if (!MainCharacter) return;
	MainCharacter->TryTransitionToState(EPlayerState::Idle);
}
