#pragma once

#include "Components/InventoryComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interacting/InteractComponent.h"
#include "Door.generated.h"

class AKeyRing;

UCLASS()
class TWOPONEC_API ADoor : public AActor
{
	GENERATED_BODY()

public:

	ADoor();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Door")
	void OpenDoorAnimation();
	UPROPERTY(EditAnywhere, Category = "Door")
	FName RequiredKeyID = "Unnamed";
	
private:
	
	UPROPERTY(VisibleAnywhere, Category = "Door")
	UStaticMeshComponent* DoorPivotMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> DoorMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInteractComponent> InteractableComp;
	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenAngle = 90.0f;
	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenDuration = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Door")
	bool bIsLocked = false;
	UPROPERTY(VisibleAnywhere, Category = "Door")
	bool bIsOpen = false;
	
	UFUNCTION(BlueprintCallable, Category = "Door")
	void OnInteracted(AActor* Interactor);
	void OpenDoor();
	UFUNCTION(BlueprintCallable, Category = "Door")
	void TryOpenDoor(AActor* Interactor);
	
	
	
};
