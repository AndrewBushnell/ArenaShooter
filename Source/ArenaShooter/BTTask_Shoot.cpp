#include "BTTask_Shoot.h"
#include "AIController.h"
#include "ArenaEnemy.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

// Tells the enemy to start shooting
EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AArenaEnemy* Enemy = AIController ? Cast<AArenaEnemy>(AIController -> GetPawn()) : nullptr;

	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	Enemy -> StartShooting();
	return EBTNodeResult::Succeeded;
}