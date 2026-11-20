#include "BTTask_SetFocusTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetFocusTarget::UBTTask_SetFocusTarget()
{
	NodeName = TEXT("Set Focus");

	// Only lets you pick actor keys in the behavior tree
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetFocusTarget, BlackboardKey), AActor::StaticClass());
}

// Makes the AI turn and face the target
EBTNodeResult::Type UBTTask_SetFocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent() -> GetValueAsObject(BlackboardKey.SelectedKeyName));

	if (!AIController || !Target)
	{
		return EBTNodeResult::Failed;
	}

	AIController -> SetFocus(Target);
	return EBTNodeResult::Succeeded;
}