#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StopShooting.generated.h"

UCLASS()
class ARENASHOOTER_API UBTTask_StopShooting : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_StopShooting();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};