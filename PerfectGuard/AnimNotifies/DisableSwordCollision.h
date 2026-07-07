#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DisableSwordCollision.generated.h"


UCLASS()
class CS_API UDisableSwordCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSequence, const
		FAnimNotifyEventReference& EventReference) override;
	
};
