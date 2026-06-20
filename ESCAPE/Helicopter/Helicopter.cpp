#include "Helicopter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AHelicopter::AHelicopter()
{
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100.0f;
	
	CurrentHealth = MaxHealth;
}

void AHelicopter::BeginPlay()
{
	Super::BeginPlay();
}

void AHelicopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 1);
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is null!"));
		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector Offset(-1500.f, 0.f, 1300.f);
	FVector TargetLocation = PlayerLocation + Offset;
	FVector NewLocation = FMath::VInterpTo(
		GetActorLocation(),
		TargetLocation,
		DeltaTime,
		0.8f 
	);

	SetActorLocation(NewLocation);

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(
		NewLocation,
		PlayerLocation
	);
	FRotator NewRotation = FMath::RInterpTo(
		GetActorRotation(),
		LookAt,
		DeltaTime,
		2.0f
	);

	SetActorRotation(NewRotation);
}

float AHelicopter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
						  AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = FMath::Clamp(DamageAmount, 0.f, CurrentHealth);

	CurrentHealth -= DamageApplied;

	UE_LOG(LogTemp, Warning, TEXT("Helicopter took %f damage! Health now: %f"), DamageApplied, CurrentHealth);

	if (CurrentHealth <= 0.f)
	{
		HelicopterDie();
	}

	return DamageApplied;
}

void AHelicopter::HelicopterDie_Implementation()
{
}

