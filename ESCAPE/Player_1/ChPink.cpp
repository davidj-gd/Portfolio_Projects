

#include "ChPink.h"

#include "Chase/BikeChaseComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Pistol/Pistol.h"


AChPink::AChPink()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	InteractComponent = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComponent"));
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	
	bIsAiming = false;
	bFoundPistol = false;

}

void AChPink::BeginPlay()
{
	Super::BeginPlay();
	
	bIsInteracting = false;
	InteractComponent->bIsInteracting = false;
	
	BikeChaseComponent = FindComponentByClass<UBikeChaseComponent>();
	if (PistolClass)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;

		PistolInstance = GetWorld()->SpawnActor<APistol>(
			PistolClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			Params
		);
		
		// We also find the camera here
		ChaseCamera = FindComponentByClass<UCameraComponent>();
	}
}

void AChPink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChPink::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AChPink::Move);
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AChPink::Look);
		EIC->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AChPink::MouseLook);
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &AChPink::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &AChPink::StopJump);
		EIC->BindAction(RunAction, ETriggerEvent::Started, this, &AChPink::Run);
		EIC->BindAction(RunAction, ETriggerEvent::Completed, this, &AChPink::StopRun);
		EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AChPink::OnInteractPressed);
		EIC->BindAction(CraneAction, ETriggerEvent::Triggered, this, &AChPink::OnCraneInput);
		EIC->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AChPink::OnShoot);
		EIC->BindAction(AimAction, ETriggerEvent::Started, this, &AChPink::OnAimPressed);
		EIC->BindAction(AimAction, ETriggerEvent::Completed, this, &AChPink::OnAimReleased);
	}
}

#pragma region Input Actions

void AChPink::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	DoMove(Movement.X, Movement.Y);
}

void AChPink::Look(const FInputActionValue& Value)
{
	if (InteractComponent && InteractComponent->bIsInteracting) return;
	FVector2D Look = Value.Get<FVector2D>();
	DoLook(Look.X, Look.Y);
}

void AChPink::MouseLook(const FInputActionValue& Value)
{
	if (InteractComponent && InteractComponent->bIsInteracting) return;
	const FVector2D Look = Value.Get<FVector2D>();
	DoLook(Look.X * MouseLookSensitivity.X, Look.Y * MouseLookSensitivity.Y);
}

void AChPink::Jump()
{
	Super::Jump();
}

void AChPink::StopJump()
{
}

void AChPink::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AChPink::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

void AChPink::OnInteractPressed()
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

void AChPink::OnCraneInput(const FInputActionValue& Value)
{
	InteractComponent->HandleInput(Value);
}

void AChPink::DoMove(float Right, float Forward)
{
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation (0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}

void AChPink::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

#pragma endregion

#pragma region Chase

void AChPink::ChangeInputMapping()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP) return;
	UEnhancedInputLocalPlayerSubsystem* Subsys =
		LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsys) return;
	if (InputContext) Subsys->RemoveMappingContext(InputContext);
	if (BikeInputContext) Subsys->AddMappingContext(BikeInputContext, 1);
}

void AChPink::AttachToBike(USceneComponent* SeatComponent)
{
	if (!IsValid(SeatComponent))
	{
		return;
	}

	ChaseBike = SeatComponent->GetOwner();

	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		// Set new movement/rotation settings during the bike segment
		Move->StopMovementImmediately();
		Move->DisableMovement();
		AChPink::bUseControllerRotationYaw = false;
	}
	
	AChPink::SetActorEnableCollision(false);

	// Attach player to the bike seat
	SetActorLocationAndRotation(
		SeatComponent->GetComponentLocation(),
		SeatComponent->GetComponentRotation(),
		false,
		nullptr,
		ETeleportType::TeleportPhysics);

	AttachToComponent(
		SeatComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	Camera->SetActive(false);
	
	AttachPistolToHand();
}
#pragma endregion

#pragma region Combat

void AChPink::OnAimPressed()
{
	bIsAiming = true;
	StartAim();
}

void AChPink::OnAimReleased()
{
	bIsAiming = false;
	StopAim();
}

void AChPink::OnShoot()
{
	if (!PistolInstance || !ChaseCamera) return;

	FVector CamLocation = ChaseCamera->GetComponentLocation();
	FVector CamForward = ChaseCamera->GetForwardVector();

	PistolInstance->ShootPistol(CamLocation, CamForward);
}
 
void AChPink::CheckForPistol()
{
	bFoundPistol = true;
}

void AChPink::StopAim_Implementation()
{
}

void AChPink::StartAim_Implementation()
{
}

// We set this in Blueprint
void AChPink::AttachPistolToHand_Implementation()
{
}

#pragma endregion



