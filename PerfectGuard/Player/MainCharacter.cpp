#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "../Components/LockOnComponent.h"
#include "../AI/Enemy.h"
#include "CS/Components/CombatComponent.h"
#include "CS/Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 175.f;
	GetCharacterMovement()->MaxAcceleration = 800.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	bUseControllerRotationYaw = false;
	
	// Sword collider - We attach them to each hand of the player (Dual sword)
	LeftSwordCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftSwordCollider"));
	LeftSwordCollider->SetupAttachment(GetMesh(), FName("hand_l"));
	RightSwordCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightSwordCollider"));
	RightSwordCollider->SetupAttachment(GetMesh(), FName("hand_r"));
	
	// Begin state
	CurrentState = EPlayerState::Idle;
	
	// Components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("CameraLockOnComponent"));
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!LocalPlayerSubsystem) return;
	
	if (InputMappingContext)
	{
		LocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
	}
	
	LeftSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightSwordCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const float TargetSpeed = bIsRunning ? 600.f : 175.f;
	GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(GetCharacterMovement()->MaxWalkSpeed, TargetSpeed, DeltaTime, WalkRunInterpSpeed);
	
	// Face towards enemy if we are currently attacking and not locked on
	if (CurrentState == EPlayerState::Attacking && !LockOnComponent->bIsLockedOn && AttackActor && !CombatComponent->bIsSuperAttacking)
	{
		FVector Target = AttackActor->GetActorLocation() - GetActorLocation();
		Target.Z = 0.f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Target.Rotation(), DeltaTime, 12.f));
	}
}
 
#pragma region Inputs

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!InputComp) return;
	
	InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
	InputComp->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainCharacter::StopMove);
	InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
	InputComp->BindAction(AttackAction, ETriggerEvent::Started, this, &AMainCharacter::Attack);
	InputComp->BindAction(DodgeAction, ETriggerEvent::Started, this, &AMainCharacter::Dodge);
	InputComp->BindAction(CamLockOnAction, ETriggerEvent::Started, this, &AMainCharacter::LockOnTarget);
	InputComp->BindAction(RunAction, ETriggerEvent::Triggered, this, &AMainCharacter::Run);
	InputComp->BindAction(RunAction, ETriggerEvent::Completed, this, &AMainCharacter::StopRun);
	InputComp->BindAction(ParryAction, ETriggerEvent::Started, this, &AMainCharacter::Parry);
	InputComp->BindAction(ParryAction, ETriggerEvent::Completed, this, &AMainCharacter::StopParry);
	InputComp->BindAction(SwitchLockOnTargetAction, ETriggerEvent::Triggered, this, &AMainCharacter::SwitchLockOnTarget);
	InputComp->BindAction(SuperAttackAction, ETriggerEvent::Started, this, &AMainCharacter::SuperAttack);
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	if (CurrentState == EPlayerState::Idle)
	{
		TryTransitionToState(EPlayerState::Moving);
	}
	
	const FVector2D Input = Value.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	// For Dodge
	LastMoveInput = Input;
	
	AddMovementInput(Forward, Input.Y);
	AddMovementInput(Right, Input.X);
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	// No manual camera control while locked on — the lock owns the camera.
	if (LockOnComponent->bIsLockedOn) return;

	const FVector2D LookInput = Value.Get<FVector2D>();
	AddControllerPitchInput(LookInput.Y);
	AddControllerYawInput(LookInput.X);
}

void AMainCharacter::StopMove()
{
	if (CurrentState == EPlayerState::Moving)
	{
		TryTransitionToState(EPlayerState::Idle);
	}
}

void AMainCharacter::Attack()
{
	if (TryTransitionToState(EPlayerState::Attacking))
	{
		AttackActor = FindAttackTarget();
		CombatComponent->DoAttack();
		CombatComponent->bIsParrying = false;
	}
	else if (CombatComponent->bComboWindowOpen)
	{
		CombatComponent->DoAttack();
	}
}

void AMainCharacter::Dodge()
{
	if (!TryTransitionToState(EPlayerState::Dodging)) return;
	UAnimMontage* ChosenDirectionMontage = DodgeBackward;
	if (!LastMoveInput.IsNearlyZero())
	{
		const float Angle = FMath::RadiansToDegrees(
			FMath::Atan2(LastMoveInput.X, LastMoveInput.Y));
		if (Angle >= -45.f && Angle <= 45.f)        ChosenDirectionMontage = DodgeForward;
		else if (Angle > 45.f && Angle < 135.f)      ChosenDirectionMontage = DodgeRight;
		else if (Angle < -45.f && Angle > -135.f)    ChosenDirectionMontage = DodgeLeft;
	}
	
	PlayAnimMontage(ChosenDirectionMontage);

}

void AMainCharacter::LockOnTarget()
{
	if (!LockOnComponent->bIsLockedOn)
	{
		LockOnComponent->EnterLockOn();
	}
	else
	{
		LockOnComponent->ExitLockOn();
	}
}

void AMainCharacter::Run()
{
	if (CombatComponent->bIsParrying) return;
	bIsRunning = true;
}

void AMainCharacter::StopRun()
{
	bIsRunning = false;
}

void AMainCharacter::Parry()
{
	if (TryTransitionToState(EPlayerState::Parry))
	{
		CombatComponent->DoParry();
	}
}

void AMainCharacter::StopParry()
{
	if (CurrentState != EPlayerState::Parry) return;
	TryTransitionToState(EPlayerState::Idle);
	CombatComponent->DoStopParry();
}

void AMainCharacter::SwitchLockOnTarget(const FInputActionValue& Value)
{
	if (!LockOnComponent->bIsLockedOn) return;
	const float MouseX = Value.Get<float>();
	if (FMath::Abs(MouseX) < LockOnComponent->MouseSwitchThreshold)
	{
		LockOnComponent->bCanSwitchTarget = true;
		return;
	}
	if (!LockOnComponent->bCanSwitchTarget) return;
	LockOnComponent->SwitchTarget(MouseX > 0.f ? 1.f : -1.f);
	LockOnComponent->bCanSwitchTarget = false;
}

void AMainCharacter::SuperAttack()
{
	if (TryTransitionToState(EPlayerState::Attacking))
	{
		CaptureFinisherTarget();
		CombatComponent->DoSuperAttack();
		LockOnComponent->ExitLockOn();
	}
}

void AMainCharacter::EndSuperAttack()
{
	ReleaseFinisherTarget();   // let the captured enemy move/think again

	if (TryTransitionToState(EPlayerState::Idle))
	{
		CombatComponent->bIsSuperAttacking = false;
		if (LockOnComponent->bIsLockedOn)
		{
			LockOnComponent->EnterLockOn();
		}

	}
}

void AMainCharacter::CaptureFinisherTarget()
{
	TArray<AActor*> Found;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), FinisherSearchRadius,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), AEnemy::StaticClass(), TArray<AActor*>(), Found);

	AEnemy* Nearest = nullptr;
	float BestDist = MAX_FLT;
	for (AActor* Actor : Found)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (!Enemy || Enemy->GetHealth() <= 0.f) continue;
		const float Dist = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
		if (Dist < BestDist) { BestDist = Dist; Nearest = Enemy; }
	}
	if (!Nearest) return;

	FVector ToEnemy = Nearest->GetActorLocation() - GetActorLocation();
	ToEnemy.Z = 0.f;
	SetActorRotation(ToEnemy.Rotation());

	FVector CaptureLoc = GetActorLocation() + GetActorForwardVector() * FinisherDistance;
	CaptureLoc.Z = Nearest->GetActorLocation().Z;

	FRotator FaceMe = (GetActorLocation() - CaptureLoc).Rotation();
	FaceMe.Pitch = 0.f;
	FaceMe.Roll = 0.f;

	Nearest->CaptureForFinisher(CaptureLoc, FaceMe);
	CapturedEnemy = Nearest;
}

void AMainCharacter::ReleaseFinisherTarget()
{
	if (CapturedEnemy)
	{
		CapturedEnemy->ReleaseFromFinisher();
		CapturedEnemy = nullptr;
	}
}

#pragma endregion

#pragma region Health/Damage handle & rules

void AMainCharacter::HandleDeath()
{
	TryTransitionToState(EPlayerState::Dead);
	LockOnComponent->ExitLockOn();
	PlayAnimMontage(DeathMontage);
	ViewDeathScreen();
}

void AMainCharacter::ReceiveDamage(float Amount)
{
	if (CombatComponent->bPerfectParry)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PerfectParrySound, GetActorLocation());
		SpawnParryEffect();
		ApplyHitStop(0.1f);
		return;
	}
	
	CombatComponent->DisableCollision();
	
	// Forward new amount of damaged based on if we're parrying or not
	float FinalAmount = CombatComponent->bIsParrying ? Amount * 0.7f : Amount;
	if (CombatComponent->bIsParrying)
	{
		SpawnParryEffect();
		PlayAnimMontage(ParryBreakMontage);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ParryBreakSound, GetActorLocation());
		TryTransitionToState(EPlayerState::Idle);
	}
	else if (CombatComponent->AttackIndex >= 3)
	{
		if (TryTransitionToState(EPlayerState::Hitstun))
		{
			PlayAnimMontage(HeavyHitstunMontage);
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
			if (BloodSplatter.Num() > 0)
			{
				int32 Index = FMath::RandRange(0, BloodSplatter.Num() - 1);
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatter[Index], GetActorLocation());
			}
			HealthComponent->TakeDamage(Amount);
			return;
		}
	}
	else if (TryTransitionToState(EPlayerState::Hitstun))
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitstunSound, GetActorLocation());
		if (HitstunAnimIndex >= HitstunAnimMontage.Num()) HitstunAnimIndex = 0;
		const float Duration = PlayAnimMontage(HitstunAnimMontage[HitstunAnimIndex]);
		HitstunAnimIndex++;
		if (BloodSplatter.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, BloodSplatter.Num() - 1);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSplatter[Index], GetActorLocation());
		}
		
		// Safety incase we get stuck in state
		GetWorldTimerManager().SetTimer(HitstunRecoverHandle, [this]()
		{
			if (CurrentState == EPlayerState::Hitstun)
			{
				TryTransitionToState(EPlayerState::Idle);
			}
		}, Duration, false);
	}
	
	// Send damage amount to health component
	HealthComponent->TakeDamage(FinalAmount);
}

#pragma endregion

bool AMainCharacter::TryTransitionToState(EPlayerState NewState)
{
	if (CanTransitionToState(NewState))
	{
		CurrentState = NewState;
		OnEnterState(NewState);
		return true;
	}
	return false;
}

void AMainCharacter::OnEnterState(EPlayerState State)
{
	if (State != EPlayerState::Parry)
	{
		CombatComponent->bIsParrying = false;
		CombatComponent->bPerfectParry = false;
	}
	
	if (State != EPlayerState::Attacking)
	{
		CombatComponent->bComboWindowOpen = false;
		CombatComponent->bComboBufferAttack = false;
	}
	
}

bool AMainCharacter::CanTransitionToState(EPlayerState NewState) const
{
	switch (CurrentState)
	{
		case EPlayerState::Idle:
			return (NewState == EPlayerState::Moving
			 || NewState == EPlayerState::Attacking
			 || NewState == EPlayerState::Dodging
			 || NewState == EPlayerState::Hitstun
			 || NewState == EPlayerState::Dead
			 || NewState == EPlayerState::Parry);
			
		case EPlayerState::Moving:
			return (NewState == EPlayerState::Moving
			|| NewState == EPlayerState::Attacking
			|| NewState == EPlayerState::Dodging
			|| NewState == EPlayerState::Hitstun
			|| NewState == EPlayerState::Dead
			|| NewState == EPlayerState::Idle
			|| NewState == EPlayerState::Parry);
			
		case EPlayerState::Attacking:
			return (NewState == EPlayerState::Hitstun
				|| (NewState == EPlayerState::Idle)
				|| NewState == EPlayerState::Dead);
		
		case EPlayerState::Dodging:
			return (NewState == EPlayerState::Hitstun
				|| NewState == EPlayerState::Dead
				|| NewState == EPlayerState::Idle
				|| NewState == EPlayerState::Moving);
			
		case EPlayerState::Hitstun:
			return (NewState == EPlayerState::Hitstun
				|| NewState == EPlayerState::Dead
				|| NewState == EPlayerState::Idle);
		
		case EPlayerState::Parry:
			return (NewState == EPlayerState::Moving
				|| NewState == EPlayerState::Dead
				|| NewState == EPlayerState::Parry
				|| NewState == EPlayerState::Hitstun
				|| NewState == EPlayerState::Idle);
		
		case EPlayerState::Dead:
			return (NewState == EPlayerState::Dead);
	
		default: return false;
	}
}

AActor* AMainCharacter::FindAttackTarget()
{
	TArray<AActor*> FoundActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), CombatComponent->AttackTargetRange,
		TArray<TEnumAsByte<EObjectTypeQuery>>(), AEnemy::StaticClass(), TArray<AActor*>(), FoundActors);
	
	FVector CamForward = Camera->GetForwardVector();
	CamForward.Z = 0.f;
	CamForward.Normalize();
	
	AActor* BestActor = nullptr;
	float BestScore = -1.f;
	for (AActor* Actor : FoundActors)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (!Enemy || Enemy->GetHealth() <= 0.f) continue;
		FVector ToEnemy = Actor->GetActorLocation() - GetActorLocation();
		ToEnemy.Z = 0.f;
		const float Distance = ToEnemy.Size();
		const FVector Direction = ToEnemy.GetSafeNormal();
		
		const float Facing = FVector::DotProduct(Direction, CamForward);
		
		if (Facing < 0.3f) continue;
		
		const float Score = Facing / Distance;
		if (Score > BestScore)
		{
			BestScore = Score;
			BestActor = Enemy;
		}
		
	}
	return BestActor;
}

void AMainCharacter::ViewDeathScreen_Implementation()
{
}

void AMainCharacter::DoCameraShake_Implementation()
{
}


void AMainCharacter::SwordTrailStart_Implementation()
{
}

void AMainCharacter::SwordTrailEnd_Implementation()
{
}

void AMainCharacter::SpawnParryEffect_Implementation()
{
}

UCombatComponent* AMainCharacter::GetCombatComponent() const
{
	return CombatComponent;
}
