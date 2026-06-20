
#include "Sword.h"

ASword::ASword()
{

	PrimaryActorTick.bCanEverTick = true;
	
	WeaponMesh = CreateDefaultSubobject<UStaticMesh>(TEXT("Sword Mesh"));
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

