#include "AnimNotify_WeaponTrace.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"

void UAnimNotify_WeaponTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	
	if (UWeaponComponent* WeaponComp = Owner->FindComponentByClass<UWeaponComponent>())
	{
		WeaponComp->DoAttackTrace(TraceSocket, TraceRadius);
	}
	
}
