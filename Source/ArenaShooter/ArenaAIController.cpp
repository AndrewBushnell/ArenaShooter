#include "ArenaAIController.h"
#include "ArenaCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AArenaAIController::AArenaAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Sight sense setup, how far and how wide the AI can see
	SightConfig -> SightRadius = 2000.f;
	SightConfig -> LoseSightRadius = 2500.f;
	SightConfig -> PeripheralVisionAngleDegrees = 70.f;
	SightConfig -> SetMaxAge(5.f);
	SightConfig -> DetectionByAffiliation.bDetectEnemies = true;
	SightConfig -> DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig -> DetectionByAffiliation.bDetectFriendlies = true;

	AIPerception -> ConfigureSense(*SightConfig);
	AIPerception -> SetDominantSense(SightConfig->GetSenseImplementation());
	SetPerceptionComponent(*AIPerception);
}

// Starts the behavior tree once the AI takes control of the enemy
void AArenaAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AIPerception -> OnTargetPerceptionUpdated.AddDynamic(this, &AArenaAIController::OnTargetPerceived);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

// Sets the player as the target when seen and clears it when sight is lost
void AArenaAIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	// Only care about the player
	if (!Cast<AArenaCharacter>(Actor))
	{
		return;
	}

	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComp -> SetValueAsObject(TEXT("TargetActor"), Actor);
		}
		else
		{
			BlackboardComp -> ClearValue(TEXT("TargetActor"));
		}
	}
}