
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class TWOPONEC_API ASword : public AActor
{
	GENERATED_BODY()

public:

	ASword();

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UStaticMesh* WeaponMesh;

public:

	virtual void Tick(float DeltaTime) override;
};
