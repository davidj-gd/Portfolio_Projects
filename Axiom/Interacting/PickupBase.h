#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/InventoryComponent.h"
#include "PickupBase.generated.h"

UCLASS()
class TWOPONEC_API APickupBase : public AActor
{
	GENERATED_BODY()

public:

	APickupBase();

protected:

	virtual void BeginPlay() override;

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	FName PickupID = "Unnamed";
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	bool bIsCollected = false;
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	TObjectPtr<UStaticMeshComponent> PickupMesh;
	UPROPERTY(EditAnywhere, Category = "Pickup")
	USphereComponent* CollisionComp;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void Collect(AActor* Collector);
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPickupCollected();
 	UFUNCTION(BlueprintPure, Category = "Pickup") bool IsAvailable() const;
	
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);
};
