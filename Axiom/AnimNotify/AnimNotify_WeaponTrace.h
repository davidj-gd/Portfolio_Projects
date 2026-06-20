
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_WeaponTrace.generated.h"


UCLASS()
class TWOPONEC_API UAnimNotify_WeaponTrace : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, Category = "Trace")
	FName TraceSocket = "hand_weapon";
	UPROPERTY(EditAnywhere, Category = "Trace", meta = (Units = "cm"))
	float TraceRadius = 100.0f;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;
	
};
