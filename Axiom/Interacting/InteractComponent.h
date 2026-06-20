#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteracted, AActor*, Interactor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInteractComponent();

protected:

	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void Interact(AActor* Interactor);
	UPROPERTY(BlueprintAssignable, Category = "Interact")
	FOnInteracted OnInteract;
	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	FText InteractPrompt;
	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	bool bIsEnabled = true;
	
};
