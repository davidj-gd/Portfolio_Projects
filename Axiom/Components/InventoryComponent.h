#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TWOPONEC_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();
	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FName> CollectedKeys;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddKey(FName KeyID);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasKey(FName KeyID) const;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseKey(FName KeyID);
};
