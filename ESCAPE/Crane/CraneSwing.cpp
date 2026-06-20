
#include "CraneSwing.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "ChDark/ChDark.h"

ACraneSwing::ACraneSwing()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CraneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CraneRoot"));
	RootComponent = CraneRoot;

	InteractionSeat = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionSeat"));
	InteractionSeat->SetupAttachment(CraneRoot);
}

void ACraneSwing::BeginPlay()
{
	Super::BeginPlay();
}

void ACraneSwing::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACraneSwing::OnInteractStart(AActor* InteractingActor)
{
	if (AChDark* P2 = Cast<AChDark>(InteractingActor))
	{
		P2->bIsHanging = true;
	}
	
	CurrentUser = InteractingActor;
	APawn* Pawn = Cast<APawn>(InteractingActor);
	if (!Pawn) return;

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC) return;

	ACharacter* Character = Cast<ACharacter>(Pawn);

	if (UCharacterMovementComponent* MoveComp = Pawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		MoveComp->DisableMovement();
		MoveComp->bOrientRotationToMovement = false;
	}

	if (Character)
	{
		bCachedUseControllerRotationYaw = Character->bUseControllerRotationYaw;
		Character->bUseControllerRotationYaw = false;
		bHasCachedCharacterRotationSettings = true;
	}

	if (InteractionSeat)
	{
		Pawn->SetActorLocationAndRotation(
			InteractionSeat->GetComponentLocation(),
			InteractionSeat->GetComponentRotation(),
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);
		Pawn->AttachToComponent(InteractionSeat, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	
	PC->SetIgnoreLookInput(true);
	PC->SetViewTargetWithBlend(this, 0.5f);
}

void ACraneSwing::OnReceiveInput(const FInputActionValue& Value)
{
}

void ACraneSwing::OnInteractEnd(AActor* InteractingActor)
{
	if (AChDark* P2 = Cast<AChDark>(InteractingActor))
	{
		P2->bIsHanging = false;
	}
	
	APawn* Pawn = Cast<APawn>(InteractingActor);
	if (!Pawn) return;

	if (UCharacterMovementComponent* MoveComp = Pawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
		MoveComp->bOrientRotationToMovement = false;
	}

	if (ACharacter* Character = Cast<ACharacter>(Pawn))
	{
		if (bHasCachedCharacterRotationSettings)
		{
			Character->bUseControllerRotationYaw = bCachedUseControllerRotationYaw;
		}
	}

	Pawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(InteractingActor, 0.5f);
		PC->SetIgnoreLookInput(false);
	}

	CurrentUser = nullptr;
	bHasCachedCharacterRotationSettings = false;
}

void ACraneSwing::TriggerCraneSwing(AActor* InteractingActor)
{
	OnInteractStart(InteractingActor);
}
