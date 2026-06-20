
#include "Interactable.h"
#include "Interactable/InteractComponent.h"
#include "Interactable/Interacting.h"
#include "GameFramework/Character.h"

namespace
{
	/** Stops Cast asserts when a level reference is missing or points at a CDO (common cause of Default__Object → Actor crash). */
	bool IsPlacedActor(AActor* Actor)
	{
		return Actor != nullptr && IsValid(Actor) && !Actor->HasAnyFlags(RF_ClassDefaultObject);
	}
}

AInteractable::AInteractable()
{

	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(50.0f);
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Sphere->SetGenerateOverlapEvents(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractable::OnCollisionOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AInteractable::OnCollisionEndOverlap);
	bPlayerCanInteract = false;
}

void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractable::OnCollisionOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (!IsPlacedActor(OtherActor))
	{
		return;
	}
	
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (UInteractComponent* InteractComp = Player->FindComponentByClass<UInteractComponent>())
		{
			IInteracting* TargetInterface = nullptr;
			if (IsPlacedActor(InteractionTarget))
			{
				TargetInterface = Cast<IInteracting>(InteractionTarget);
			}
			InteractComp->SetCurrentInteractable(TargetInterface);
		}
	}
	
	bPlayerCanInteract = IsPlacedActor(InteractionTarget) && (Cast<IInteracting>(InteractionTarget) != nullptr);
	UE_LOG(LogTemp, Warning, TEXT("Overlapped!"), *OtherActor->GetName());
}

void AInteractable::OnCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsPlacedActor(OtherActor))
	{
		return;
	}
	bPlayerCanInteract = false;
	
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (UInteractComponent* InteractComp = Player->FindComponentByClass<UInteractComponent>())
		{
			IInteracting* TargetInterface = nullptr;
			if (IsPlacedActor(InteractionTarget))
			{
				TargetInterface = Cast<IInteracting>(InteractionTarget);
			}
			InteractComp->ClearCurrentInteractable(TargetInterface);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Stepped Out!"), *OtherActor->GetName());
}
