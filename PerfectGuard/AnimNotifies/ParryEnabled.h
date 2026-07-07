#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ParryEnabled.generated.h"


UCLASS()
class CS_API UParryEnabled : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const
		FAnimNotifyEventReference& EventReference) override;
};
