#include "CanAttack.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WeaponComponent.h"

void UCanAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	if (UWeaponComponent* WeaponComp = Owner->FindComponentByClass<UWeaponComponent>())
	{
		WeaponComp->SetCanAttack(true);
	}
}
