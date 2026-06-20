#include "PickupBase.h"

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	RootComponent = PickupMesh;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>("CollisionComp");
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->InitSphereRadius(200.f);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnOverlapBegin);
}

void APickupBase::Collect(AActor* Collector)
{
	if (bIsCollected || !Collector) return;
	bIsCollected = true;

	if (UInventoryComponent* PlayerInventory = Collector->FindComponentByClass<UInventoryComponent>())
	{
		PlayerInventory->AddKey(PickupID);
	}

	OnPickupCollected();
}

bool APickupBase::IsAvailable() const
{
	return !bIsCollected;
}

void APickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	
	if (OtherActor->ActorHasTag("Player"))
	{
		Collect(OtherActor);
	}
}

