// Fill out your copyright notice in the Description page of Project Settings.


#include "ChDark.h"


#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"


AChDark::AChDark()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AChDark::BeginPlay()
{
	Super::BeginPlay();
	
	bIsInteracting = false;
	InteractComponent->bIsInteracting = false;
	bIsPressingButton = false;
	bIsHanging = false;
	
}

void AChDark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChDark::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AChDark::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChDark::Look);
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AChDark::MouseLook);
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AChDark::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AChDark::StopJump);
		EIC->BindAction(RunAction, ETriggerEvent::Started, this, &AChDark::Run);
		EIC->BindAction(RunAction, ETriggerEvent::Completed, this, &AChDark::StopRun);
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AChDark::OnInteractPressed);
	}
	
}

void AChDark::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	DoMove(Movement.X, Movement.Y);
}

void AChDark::Look(const FInputActionValue& Value)
{
	if (InteractComponent && InteractComponent->bIsInteracting) return;
	FVector2D Look = Value.Get<FVector2D>();
	DoLook(Look.X, Look.Y);
}

void AChDark::MouseLook(const FInputActionValue& Value)
{
	if (InteractComponent && InteractComponent->bIsInteracting) return;
	const FVector2D Look = Value.Get<FVector2D>();
	DoLook(Look.X * MouseLookSensitivity.X, Look.Y * MouseLookSensitivity.Y);
}

void AChDark::Jump()
{
	Super::Jump();
}

void AChDark::StopJump()
{
}

void AChDark::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AChDark::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AChDark::OnInteractPressed()
{
	if (!InteractComponent->bIsInteracting)
	{
		bOrientRotationToMovementBeforeInteract = GetCharacterMovement()->bOrientRotationToMovement;
		InteractComponent->BeginInteraction();
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		InteractComponent->EndInteraction();
		GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovementBeforeInteract;
	}
}

void AChDark::DoMove(float Right, float Forward)
{
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation (0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void AChDark::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}
