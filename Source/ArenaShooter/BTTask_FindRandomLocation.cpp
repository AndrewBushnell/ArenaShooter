#include "BTTask_FindRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	// Only lets you pick vector keys in the behavior tree
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindRandomLocation, BlackboardKey));
}

// Picks a random spot on the nav mesh and saves it to MoveLocation
EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController ? AIController -> GetPawn() : nullptr;

	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent<UNavigationSystemV1>(Pawn->GetWorld());
	FNavLocation RandomLocation;

	if (NavSystem && NavSystem -> GetRandomReachablePointInRadius(Pawn->GetActorLocation(), SearchRadius, RandomLocation))
	{
		OwnerComp.GetBlackboardComponent() -> SetValueAsVector(BlackboardKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}