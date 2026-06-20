// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerButton.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "ChDark/ChDark.h"
#include "Interactable/InteractComponent.h"
#include "Animation/AnimMontage.h"


// Sets default values
APowerButton::APowerButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ButtonRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CraneRoot"));
	RootComponent = ButtonRoot;

	InteractionSeat = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionSeat"));
	InteractionSeat->SetupAttachment(ButtonRoot);
	
}

// Called when the game starts or when spawned
void APowerButton::BeginPlay()
{
	Super::BeginPlay();
	
	bActivatedPower = false;
	
}

// Called every frame
void APowerButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APowerButton::OnInteractStart(AActor* InteractingActor)
{
	if (AChDark* P2 = Cast<AChDark>(InteractingActor))
	{
		P2->bIsPressingButton = true;
	}
	
	bActivatedPower = true;
	OnPowerActivated();

	CurrentUser = InteractingActor;
	APawn* Pawn = Cast<APawn>(InteractingActor);
	if (!Pawn) return;

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	if (!PC) return;

	ACharacter* Character = Cast<ACharacter>(Pawn);
	
	if (UCharacterMovementComponent* MoveComp = Pawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		bCachedOrientRotationToMovement = MoveComp->bOrientRotationToMovement;
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
	PC->SetViewTargetWithBlend(this, 3.0f);
	
	GetWorld()->GetTimerManager().SetTimer(ButtonTimerHandle, [this]()
	{
		if (!CurrentUser)
		{
			return;
		}
		APawn* Pawn = Cast<APawn>(CurrentUser);
		if (!Pawn)
		{
			return;
		}
		if (UInteractComponent* IC = Pawn->FindComponentByClass<UInteractComponent>())
		{
			IC->EndInteraction();
		}
	}, 6.0f, false);
	
}

void APowerButton::OnReceiveInput(const FInputActionValue& Value)
{
	
}

void APowerButton::OnInteractEnd(AActor* InteractingActor)
{
	if (AChDark* P2 = Cast<AChDark>(InteractingActor))
	{
		P2->bIsPressingButton = false;
	}

	APawn* Pawn = Cast<APawn>(InteractingActor);
	if (!Pawn) return;

	if (UCharacterMovementComponent* MoveComp = Pawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		MoveComp->SetMovementMode(MOVE_Walking);
		if (bHasCachedCharacterRotationSettings)
		{
			MoveComp->bOrientRotationToMovement = bCachedOrientRotationToMovement;
		}
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
		PC->SetIgnoreLookInput(false);
		PC->SetViewTargetWithBlend(InteractingActor, 0.5f);
	}

	CurrentUser = nullptr;
	bHasCachedCharacterRotationSettings = false;
}

void APowerButton::TemporaryTurnOffPower()
{
	bActivatedPower = false;
}

void APowerButton::TurnBackPower()
{
	bActivatedPower = true;
}

void APowerButton::OnPowerActivated_Implementation()
{
}


