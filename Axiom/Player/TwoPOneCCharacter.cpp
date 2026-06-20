
#include "TwoPOneCCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "TwoPOneC.h"

ATwoPOneCCharacter::ATwoPOneCCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilityMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Ability Mesh"));
	AbilityMesh->SetupAttachment(RootComponent);

}

void ATwoPOneCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATwoPOneCCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATwoPOneCCharacter::Look);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ATwoPOneCCharacter::StartRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ATwoPOneCCharacter::StopRun);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATwoPOneCCharacter::Look);
		
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ATwoPOneCCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATwoPOneCCharacter::OnAttack);
		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ATwoPOneCCharacter::OnUseAbility);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATwoPOneCCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTwoPOneC, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATwoPOneCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Movement speed smoothing
	float Current = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::FInterpTo(Current, CurrentSpeedTarget, DeltaTime, LerpSpeedRate);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ATwoPOneCCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilityComponent = FindComponentByClass<UAbilityComponent>();
	WeaponComponent = FindComponentByClass<UWeaponComponent>();
	InventoryComponent = FindComponentByClass<UInventoryComponent>();
	HealthComponent = FindComponentByClass<UHealthComponent>();
	CamLockOnComp = FindComponentByClass<UCameraLockOnComponent>();
	
	CurrentSpeedTarget = WalkSpeed;

	GetWorld()->GetTimerManager().SetTimer(
		InteractScanTimer,
		this,
		&ATwoPOneCCharacter::FindInteractable,
		InteractScanInterval,
		true);
}

#pragma region Standard Input (Walk/Run, Jump)

void ATwoPOneCCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATwoPOneCCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATwoPOneCCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ATwoPOneCCharacter::DoLook(float Yaw, float Pitch)
{
	// If we're locked on to an enemy, ignore Look
	if (CamLockOnComp && CamLockOnComp->IsLockedOn()) return;
	
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATwoPOneCCharacter::DoJumpStart()
{
	Jump();
}

void ATwoPOneCCharacter::DoJumpEnd()
{
	StopJumping();
}

void ATwoPOneCCharacter::StartRun()
{
	CurrentSpeedTarget = RunSpeed;
}

void ATwoPOneCCharacter::StopRun()
{
	CurrentSpeedTarget = WalkSpeed;
}

#pragma endregion

#pragma region Extra Inputs (Equip, Interact, Attack)

void ATwoPOneCCharacter::EquipWeapon()
{
	if (!WeaponComponent) return;
	if (!WeaponComponent->bHasWeaponInHand)
	{
		WeaponComponent->WeaponToHand();
	}
	else
	{
		WeaponComponent->WeaponToStash();
	}
}

void ATwoPOneCCharacter::Interact()
{
	if (FocusedInteractable && FocusedInteractable->bIsEnabled)
	{
		FocusedInteractable->Interact(this);
	}
}

void ATwoPOneCCharacter::OnUseAbility()
{
	if (!AbilityComponent) return;
	AbilityComponent->UseAbility();
}

void ATwoPOneCCharacter::OnAttack()
{
	if (!WeaponComponent) return;
	WeaponComponent->Attack();
}

#pragma endregion

void ATwoPOneCCharacter::FindInteractable()
{
	FocusedInteractable = nullptr;
	
	FVector Start = GetActorLocation() + FVector(0, 0, BaseEyeHeight);
	FVector End = Start + GetActorForwardVector() * InteractRange;
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			FocusedInteractable = HitActor->FindComponentByClass<UInteractComponent>();
		}
	}
}

void ATwoPOneCCharacter::ApplyDamage(float Amount, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(Amount, DamageCauser);
	}
}