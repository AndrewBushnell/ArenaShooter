#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenaEnemy.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class AArenaProjectile;

UCLASS()
class ARENASHOOTER_API AArenaEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AArenaEnemy();

	// Called by the Shoot and Stop Shooting BTTasks
	void StartShooting();
	void StopShooting();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	void Shoot();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> EnemyMesh;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<USceneComponent> Muzzle;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<AArenaProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health = 100.f;

private:
	FTimerHandle ShootTimer;
	FTransform SpawnTransform;
};