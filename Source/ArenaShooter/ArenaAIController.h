#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ArenaAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;

UCLASS()
class ARENASHOOTER_API AArenaAIController : public AAIController
{
	GENERATED_BODY()

public:
	AArenaAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	// Runs when the AI sees or loses sight of something
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, Category = "AI")