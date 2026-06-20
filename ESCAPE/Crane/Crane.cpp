// Fill out your copyright notice in the Description page of Project Settings.


#include "Crane.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "PowerButton/PowerButton.h"

// Sets default values
ACrane::ACrane()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CraneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CraneRoot"));
	RootComponent = CraneRoot;

	InteractionSeat = CreateDefaultSubobject<USceneComponent>(TEXT("InteractionSeat"));
	InteractionSeat->SetupAttachment(CraneRoot);

	CraneAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("CraneAudioComp"));
	CraneAudioComp->SetupAttachment(CraneRoot);
	CraneAudioComp->bAutoActivate = false;
}

void ACrane::BeginPlay()
{
	Super::BeginPlay();
	
	bIsSteering = false;

	if (CraneAudioComp && CraneCue)
	{
		CraneAudioComp->SetSound(CraneCue);
	}
	
}

void ACrane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ACrane::IsCranePowerAllowed() const
{
	if (!bRequireActivatedPowerToSteer)
	{
		return true;
	}
	if (!IsValid(PowerButtonActor))
	{
		return false;
	}
	return PowerButtonActor->bActivatedPower;
}

void ACrane::OnInteractStart(AActor* InteractingActor)
{
	bIsSteering = true;
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
	PC->SetViewTargetWithBlend(this, 0.5f);
	SetCameraEffect();
	ShowWidget();
}

void ACrane::OnReceiveInput(const FInputActionValue& Value)
{
	if (!CurrentUser)
	{
		return;
	}

	const float Axis = Value.Get<float>();
	const bool bHasInput = !FMath::IsNearlyZero(Axis);

	if (!IsCranePowerAllowed())
	{
		SetCraneMotionSound(false);
		return;
	}

	if (bHasInput)
	{
		AddActorLocalRotation(FRotator(0, Axis * RotationSpeed * GetWorld()->DeltaTimeSeconds, 0));
	}
	SetCraneMotionSound(bHasInput);
}

void ACrane::OnInteractEnd(AActor* InteractingActor)
{
	SetCraneMotionSound(false);
	bIsSteering = false;
	
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
		PC->SetViewTargetWithBlend(InteractingActor, 0.5f);
		PC->SetIgnoreLookInput(false);
	}

	CurrentUser = nullptr;
	bHasCachedCharacterRotationSettings = false;
	
	DisableCameraEffect();
	DisableWidget();
}

void ACrane::SetCraneMotionSound(bool bPlaying)
{
	if (!CraneAudioComp || !CraneCue)
	{
		return;
	}
	if (bPlaying)
	{
		if (!CraneAudioComp->IsPlaying())
		{
			CraneAudioComp->Play();
		}
	}
	else if (CraneAudioComp->IsPlaying())
	{
		CraneAudioComp->Stop();
	}
}

void ACrane::DisableWidget_Implementation()
{
}

void ACrane::ShowWidget_Implementation()
{
}

void ACrane::DisableCameraEffect_Implementation()
{
}

void ACrane::SetCameraEffect_Implementation()
{
}
