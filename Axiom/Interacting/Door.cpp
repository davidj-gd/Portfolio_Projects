#include "Door.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;
	DoorPivotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorPivotMesh"));
	RootComponent = DoorPivotMesh;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	DoorMesh->SetupAttachment(DoorPivotMesh);
	InteractableComp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractableComp"));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComp->OnInteract.AddDynamic(this, &ADoor::OnInteracted);
	InteractableComp->InteractPrompt = FText::FromString(bIsLocked ? "Locked" : "Unlocked");
}

void ADoor::OnInteracted(AActor* Interactor)
{
	TryOpenDoor(Interactor);
}

void ADoor::OpenDoor()
{
	bIsOpen = true;
	OpenDoorAnimation();
}

void ADoor::TryOpenDoor(AActor* Interactor)
{
	if (bIsOpen || !Interactor) return;
	
	UInventoryComponent* PlayerInventory = Interactor->FindComponentByClass<UInventoryComponent>();
	
	if (!PlayerInventory) return;
	if (RequiredKeyID.IsNone()) return;
	
	if (PlayerInventory->HasKey(RequiredKeyID))
	{
		OpenDoor();
		PlayerInventory->UseKey(RequiredKeyID);
	}
}

void ADoor::OpenDoorAnimation_Implementation()
{
}

