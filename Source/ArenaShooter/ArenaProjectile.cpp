#include "ArenaProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AArenaProjectile::AArenaProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp -> InitSphereRadius(8.f);
	CollisionComp -> SetCollisionProfileName(TEXT("BlockAllDynamic"));
	CollisionComp -> SetNotifyRigidBodyCollision(true);
	CollisionComp -> OnComponentHit.AddDynamic(this, &AArenaProjectile::OnHit);
	RootComponent = CollisionComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh -> SetupAttachment(CollisionComp);
	ProjectileMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh -> SetRelativeScale3D(FVector(0.15f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		ProjectileMesh -> SetStaticMesh(SphereMesh.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement -> UpdatedComponent = CollisionComp;
	ProjectileMovement -> InitialSpeed = 3000.f;
	ProjectileMovement -> MaxSpeed = 3000.f;
	ProjectileMovement -> bRotationFollowsVelocity = true;
	ProjectileMovement -> ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;
}

void AArenaProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator())
	{
		CollisionComp -> IgnoreActorWhenMoving(GetInstigator(), true);
	}
}

void AArenaProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	Destroy();
}