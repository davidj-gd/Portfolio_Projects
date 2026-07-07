
#include "EnemyAIController.h"

#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionTypes.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1000.0f;
	SightConfig->LoseSightRadius = 1400.0f;
	SightConfig->PeripheralVisionAngleDegrees = 145.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	PerceptionComponent->ConfigureSense(*SightConfig);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(BehaviorTree);
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DecideState();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControlledPawn = Cast<AEnemy>(InPawn);
	if (!ControlledPawn) return;
	
	// Set Stats
	EngageCooldown = ControlledPawn->Stats.EngageCooldown;
	EngageRange = ControlledPawn->Stats.EngageRange;
	MidRange = ControlledPawn->Stats.MidRange;
	SurroundDistance = ControlledPawn->Stats.SurroundDistance;
	AttackChance = ControlledPawn->Stats.AttackChance;
	GuardChance = ControlledPawn->Stats.GuardChance;
	CircleChance = ControlledPawn->Stats.CircleChance;
}

void AEnemyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;
	
	ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());
	
	if (Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerActor"), Actor);
		if (EnemyChar)
		{
			EnemyChar->GetCharacterMovement()->MaxWalkSpeed = 600.f;
		}
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerActor"), nullptr);
		if (EnemyChar)
		{
			EnemyChar->GetCharacterMovement()->MaxWalkSpeed = 175.f;
		}
	}
}

void AEnemyAIController::DecideState()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;
	APawn* EnemyPawn = GetPawn();
	if (!EnemyPawn) return;
	AActor* Player = Cast<AActor>(BB->GetValueAsObject(TEXT("PlayerActor")));
	if (!Player) return;
	
	// Calculate distance and location of enemy and player
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector EnemyLocation = EnemyPawn->GetActorLocation();
	const float Distance = FVector::Dist(EnemyLocation, PlayerLocation);
	
	const FVector DirectionFromPlayer = (EnemyLocation - PlayerLocation).GetSafeNormal();
	BB->SetValueAsVector(TEXT("MoveToLocation"), PlayerLocation + DirectionFromPlayer * SurroundDistance);
	
	const EEnemyCombatState State = (EEnemyCombatState)BB->GetValueAsEnum(TEXT("CombatState"));


	const bool bInAction =
		State == EEnemyCombatState::Attack ||
		State == EEnemyCombatState::Guard ||
		State == EEnemyCombatState::Circle ||
		State == EEnemyCombatState::MidAttack ||
		State == EEnemyCombatState::Dodge;

	if (bInAction)
	{
		if (Distance > MidRange)
		{
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Approach);
		}
		return;
	}

	// FAR band -> approach the surround point
	if (Distance > MidRange)
	{
		BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Approach);
		return;
	}

	// MID band -> gap-closing lunge
	if (Distance > EngageRange)
	{
		BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::MidAttack);
		return;
	}

	// CLOSE band: just arrived -> drop into the neutral beat
	if (State == EEnemyCombatState::Approach)
	{
		BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Engaged);
		LastDecisionTime = GetWorld()->GetTimeSeconds();
		return;
	}

	// CLOSE band: in the neutral beat -> after a short cooldown, roll an action
	if (State == EEnemyCombatState::Engaged)
	{
		if (GetWorld()->GetTimeSeconds() - LastDecisionTime >= EngageCooldown)
		{
			LastDecisionTime = GetWorld()->GetTimeSeconds();
			const float Total = AttackChance + GuardChance + CircleChance;
			const float Roll  = FMath::FRandRange(0.f, Total);
			const EEnemyCombatState Next =
				(Roll < AttackChance)              ? EEnemyCombatState::Attack :
				(Roll < AttackChance + GuardChance) ? EEnemyCombatState::Guard  :
													  EEnemyCombatState::Circle;
			BB->SetValueAsEnum(TEXT("CombatState"), (uint8)Next);
		}
	}
}

void AEnemyAIController::TriggerDodge()
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
		BB->SetValueAsEnum(TEXT("CombatState"), (uint8)EEnemyCombatState::Dodge);
}

FGenericTeamId AEnemyAIController::GetGenericTeamId() const
{
	return FGenericTeamId(1);
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return (TeamAgent->GetGenericTeamId() == GetGenericTeamId())
				   ? ETeamAttitude::Friendly
				   : ETeamAttitude::Hostile;
		}
	}
	return ETeamAttitude::Neutral;
}
