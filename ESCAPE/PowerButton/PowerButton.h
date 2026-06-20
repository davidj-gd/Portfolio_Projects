#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interacting.h"
#include "Animation/AnimMontage.h"
#include "PowerButton.generated.h"

class USceneComponent;
class ACharacter;

UCLASS(BlueprintType, Blueprintable)
class HAZELIGHT_API APowerButton : public AActor, public IInteracting
{
	GENERATED_BODY()

public:

	APowerButton();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;
	
	virtual void OnInteractStart(AActor* Player) override;
	virtual void OnReceiveInput(const FInputActionValue& Value) override;
	virtual void OnInteractEnd(AActor* InteractingActor) override;

	UPROPERTY(VisibleAnywhere, Category = "Button")
	USceneComponent* ButtonRoot;
	UPROPERTY(VisibleAnywhere, Category = "Button")
	USceneComponent* InteractionSeat;
	UPROPERTY()
	AActor* CurrentUser = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ButtonPressMontage;

	/** True after the button has been used. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Power")
	bool bActivatedPower = false;

	/** Called from C++ when the button is activated (after bActivatedPower = true). Override in BP_PowerButton → right-click function → Implement event. */
	UFUNCTION(BlueprintNativeEvent, Category = "Power")
	void OnPowerActivated();
	
	UFUNCTION(BlueprintCallable, Category = "Power")
	void TemporaryTurnOffPower();
	UFUNCTION(BlueprintCallable, Category = "Power")
	void TurnBackPower();
	
	FTimerHandle ButtonTimerHandle;

	bool bHasCachedCharacterRotationSettings = false;
	bool bCachedUseControllerRotationYaw = true;
	bool bCachedOrientRotationToMovement = true;

};
