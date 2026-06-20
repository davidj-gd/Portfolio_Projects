#include "AbilityComponent.h"
#include "TwoPOneCCharacter.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ATwoPOneCCharacter>(GetOwner());
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAbilityComponent::AbilityEffect_Implementation()
{
}

void UAbilityComponent::UseAbility_Implementation()
{
}

