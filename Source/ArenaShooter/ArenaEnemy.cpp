#include "ArenaEnemy.h"
#include "ArenaAIController.h"
#include "ArenaProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AArenaEnemy::AArenaEnemy()
{
	// Use my AI controller and possess the enemy whether it's placed or spawned
	AIControllerClass = AArenaAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Enemy faces wherever the AI is focused
	bUseControllerRotationYaw = true;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh -> SetupAttachment(GetCapsuleComponent());
	EnemyMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyMesh -> SetRelativeScale3D(FVector(0.8f, 0.8f, 1.9f));

	// Temporary static mesh on the enemy, same as the player
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		EnemyMesh -> SetStaticMesh(CylinderMesh.Object);
	}

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle -> SetupAttachment(GetCapsuleComponent());
	Muzzle -> SetRelativeLocation(FVector(70.f, 0.f, 40.f));
}

// Save where the enemy started so it can respawn there
void AArenaEnemy::BeginPlay()
{
	Super::BeginPlay();
	SpawnTransform = GetActorTransform();
}

// Shoots every half second until told to stop
void AArenaEnemy::StartShooting()
{
	GetWorldTimerManager().SetTimer(ShootTimer, this, &AArenaEnemy::Shoot, 0.5f, true);
}

void AArenaEnemy::StopShooting()
{
	GetWorldTimerManager().ClearTimer(ShootTimer);
}

// Spawns a projectile aimed where the AI is looking
void AArenaEnemy::Shoot()
{
	if (!ProjectileClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	GetWorld() -> SpawnActor<AArenaProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), GetBaseAimRotation(), SpawnParams);
}

// Takes damage from ApplyDamage, when it does, it respawns a new enemy at the start point
float AArenaEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;

	if (Health <= 0.f)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld() -> SpawnActor<AArenaEnemy>(GetClass(), SpawnTransform, SpawnParams);
		Destroy();
	}

	return DamageAmount;
}