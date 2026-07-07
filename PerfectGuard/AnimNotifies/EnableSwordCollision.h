#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnableSwordCollision.generated.h"


UCLASS()
class CS_API UEnableSwordCollision : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const
		FAnimNotifyEventReference& EventReference) override;
	
};
