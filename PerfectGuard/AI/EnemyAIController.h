
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;
class AEnemy;

// Enemy states
UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	Approach,
	Engaged,
	Attack,
	Guard,
	Circle,
	MidAttack,
	Dodge
};

UCLASS()
class CS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

protected:

	virtual void BeginPlay() override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	void DecideState();

	UPROPERTY(EditDefaultsOnly, Category = AI)
	UBehaviorTree* BehaviorTree;

	// State properties
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float EngageRange = 300.f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float MidRange = 350.f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float EngageCooldown = 0.6f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float SurroundDistance = 150.f;
	UPROPERTY()
	float LastDecisionTime = 0.f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float DecisionInterval = 1.5f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float AttackChance = 50.f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float GuardChance = 25.f;
	UPROPERTY(EditDefaultsOnly, Category= AI)
	float CircleChance = 25.f;
	
public:

	virtual void Tick(float DeltaTime) override;
	void OnPossess(APawn* InPawn) override;
	UFUNCTION()
	void TriggerDodge();

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	AEnemy* ControlledPawn;
};
