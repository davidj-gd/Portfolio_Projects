#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SlowMoState.generated.h"

UCLASS()
class CS_API USlowMoState : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
	UPROPERTY(EditAnywhere, Category = SlowMo, meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float SlowAmount = 0.3f;
};
