#include "BTTask_StopShooting.h"
#include "AIController.h"
#include "ArenaEnemy.h"

UBTTask_StopShooting::UBTTask_StopShooting()
{
	NodeName = TEXT("Stop Shooting");
}

// Tells the enemy to stop shooting and stop looking at the target
EBTNodeResult::Type UBTTask_StopShooting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AArenaEnemy* Enemy = AIController ? Cast<AArenaEnemy>(AIController -> GetPawn()) : nullptr;

	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy -> StopShooting();
	AIController -> ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::Succeeded;
}