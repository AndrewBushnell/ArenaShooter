#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetFocusTarget.generated.h"

UCLASS()
class ARENASHOOTER_API UBTTask_SetFocusTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_SetFocusTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};