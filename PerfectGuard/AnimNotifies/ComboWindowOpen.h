#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ComboWindowOpen.generated.h"


UCLASS()
class CS_API UComboWindowOpen : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const
	FAnimNotifyEventReference& EventReference) override;
};
