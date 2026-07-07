#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyCollisionEnable.generated.h"

UCLASS()
class CS_API UEnemyCollisionEnable : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSequence, const
		FAnimNotifyEventReference& EventReference) override;
};
