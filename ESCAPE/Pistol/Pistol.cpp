
#include "Pistol.h"
#include "Kismet/GameplayStatics.h"


APistol::APistol()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SKPistolMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PistolBase"));
	SKPistolMesh->SetupAttachment(Root);
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
}

void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APistol::ShootPistol(const FVector& CamLocation, const FVector& CamForward)
{
	if (bIsOnCooldown) return;
	bIsOnCooldown = true;

	PlayMuzzleFlash();
	PlaySound();
	FireFromCamera(CamLocation, CamForward);
	GetWorld()->GetTimerManager().SetTimer(
		ShootingCooldownHandle,
		this,
		&APistol::ResetCooldown,
		0.1f,
		false);
}

void APistol::ResetCooldown()
{
	bIsOnCooldown = false;
}

void APistol::PlaySound()
{
	if (BulletSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			BulletSound,
			GetActorLocation()
		);
	}
}

void APistol::PlayMuzzleFlash()
{
	if (MuzzleFlashParticle)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFlashParticle,
			SKPistolMesh,
			TEXT("Muzzle"), 
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
}

void APistol::FireFromCamera(const FVector& CamLocation, const FVector& CamForward)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire from camera called!"));
	FVector Start = CamLocation;
	FVector End = Start + CamForward * 5000.f; 

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		// Apply damage
		UGameplayStatics::ApplyPointDamage(
			Hit.GetActor(),
			Damage,
			CamForward,
			Hit,
			GetOwner()->GetInstigatorController(),
			this,
			UDamageType::StaticClass()
		);
		
		// Hit Sound
		if (HitSounds.Num() > 0)
		{
			int32 Index = FMath::RandRange(0, HitSounds.Num() - 1);
			USoundBase* SelectedSound = HitSounds[Index];

			UGameplayStatics::PlaySoundAtLocation(
			this,
			SelectedSound,
			Hit.ImpactPoint,
			1.0f,                          
			FMath::FRandRange(0.9f, 1.1f)       
			);
		}
		
		// Hit FX
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ImpactFX,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation()
		);
	}
}



