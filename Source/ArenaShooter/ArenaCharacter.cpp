#include "ArenaCharacter.h"
#include "ArenaProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/StaticMesh.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

AArenaCharacter::AArenaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationYaw = true;
	GetCharacterMovement() -> bOrientRotationToMovement = false;
	GetCharacterMovement() -> MaxWalkSpeed = 600.f;
	GetCharacterMovement() -> JumpZVelocity = 600.f;
	GetCharacterMovement() -> AirControl = 0.35f;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	PlayerMesh -> SetupAttachment(GetCapsuleComponent());
	PlayerMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayerMesh -> SetRelativeScale3D(FVector(0.8f, 0.8f, 1.9f));

	// Add the temporary static mesh on the player
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		PlayerMesh -> SetStaticMesh(CylinderMesh.Object);
	}

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle -> SetupAttachment(GetCapsuleComponent());
	Muzzle -> SetRelativeLocation(FVector(70.f, 30.f, 40.f));

	// A spring arm holds the camera behind the player and rotates w the mouse
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom -> SetupAttachment(GetCapsuleComponent());
	CameraBoom -> TargetArmLength = 300.f;
	CameraBoom -> SocketOffset = FVector(0.f, 60.f, 60.f);
	CameraBoom -> bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera -> SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera -> bUsePawnControlRotation = false;
}

// Add input mapping on BeginPlay so the controls activate when the player is spawned
void AArenaCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Start the player at full health
	Health = MaxHealth;

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem -> AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

// Bind each input action to a function.
void AArenaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC -> BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArenaCharacter::Move);
		EIC -> BindAction(LookAction, ETriggerEvent::Triggered, this, &AArenaCharacter::Look);
		EIC -> BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EIC -> BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EIC -> BindAction(FireAction, ETriggerEvent::Triggered, this, &AArenaCharacter::Fire);
	}
}

// Move the player based on input values
void AArenaCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, Input.Y);
		AddMovementInput(Right, Input.X);
	}
}

// Allows the player to be rotated based on mouse movement.
void AArenaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

// Spawns a projectile when the fire input action is triggered.
void AArenaCharacter::Fire()
{
	if (!ProjectileClass)
	{
		return;
	}

	// Firing Cooldown
	const float CurrentTime = GetWorld() -> GetTimeSeconds();
	if (CurrentTime - LastFireTime < FireCooldown)
	{
		return;
	}
	LastFireTime = CurrentTime;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	GetWorld() -> SpawnActor<AArenaProjectile>(ProjectileClass, Muzzle->GetComponentLocation(), GetControlRotation(), SpawnParams);
}

// Gets called when an AI projectile hits the player through ApplyDamage
float AArenaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// Take away health but stop it from going negative
	Health = FMath::Clamp(Health - DamageTaken, 0.f, MaxHealth);

	// Display players health
	if (GEngine)
	{
		GEngine -> AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Player Health: %.0f"), Health));
	}

	return DamageTaken;
}