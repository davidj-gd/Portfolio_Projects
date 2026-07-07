#include "SlowMoState.h"
#include "Kismet/GameplayStatics.h"

void USlowMoState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!MeshComp || !MeshComp->GetWorld()) return;

	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), SlowAmount);
}

void USlowMoState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (!MeshComp || !MeshComp->GetWorld()) return;

	UGameplayStatics::SetGlobalTimeDilation(MeshComp->GetWorld(), 1.0f);
}
