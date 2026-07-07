#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ComboWindowClosed.generated.h"


UCLASS()
class CS_API UComboWindowClosed : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const
	FAnimNotifyEventReference& EventReference) override;
};
