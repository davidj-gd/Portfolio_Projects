
#include "BikeChaseComponent.h"
#include "ChPink/ChPink.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"


UBikeChaseComponent::UBikeChaseComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	bChaseSceneStarted = false;
}

void UBikeChaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Player1 = Cast<AChPink>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player2 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 1);
}

void UBikeChaseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBikeChaseComponent::StartChase()
{
	bChaseSceneStarted = true;

	if (!IsValid(Player1))
	{
		Player1 = Cast<AChPink>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	if (!IsValid(Player2))
	{
		Player2 = UGameplayStatics::GetPlayerCharacter(GetWorld(), 1);
	}

	if (IsValid(Player1))
	{
		Player1->ChaseBike = GetOwner();
		USceneComponent* Seat = Cast<USceneComponent>(PassengerSeatRef.GetComponent(GetOwner()));
		Player1->ChangeInputMapping();
		Player1->AttachToBike(Seat);
		Player1->CheckForPistol();
	}
}

void UBikeChaseComponent::EnableShootingControlls()
{

}
